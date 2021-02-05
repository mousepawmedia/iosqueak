#include "iosqueak/channel.hpp"

bool Channel::can_parse()
{
	// If we aren't sure about the parsing condition...
	if (~parse) {
		/* Proceed if the verbosity is in range
		 * and the category is set to parse. */
		parse = ((vrb <= process_vrb) && flags_check(process_cat, cat)) ? true
																		: false;
	}
	return parse;
}

void Channel::flush()
{
	/* TODO: For non-standard outputs, this should attach a bytecode to
	 * tell external outputs to flush.
	 */

	// Flush is essential for progress-style outputs (after \r and no \n)

	// Flush the standard streams.
	switch (echo_mode) {
		case IOEchoMode::cout:
			std::cout << std::flush;
			std::cerr << std::flush;
			break;
		case IOEchoMode::printf:
			fflush(stdout);
			fflush(stderr);
			break;
		case IOEchoMode::none:
			break;
	}
}

void Channel::move_cursor(const IOCursor& rhs)
{
	// TODO: Migrate to ioformat.hpp
	if (this->fmt.standard() == IOFormatStandard::ansi) {
		switch (rhs) {
			case IOCursor::left:
				// NOTE: Watch this. \x1B is allegedly equal to \e, check
				// it.
				inject("\x1B[1D");
				break;
			case IOCursor::right:
				// NOTE: Watch this \x1B is allegedly equal to \e, check it.
				inject("\x1B[1C");
				break;
			default:
				/* Can't happen unless you forgot to implement an
				 * IOCursor option! */
				assert(false);
		}
	}
}

void Channel::inject(char ch)
{
	// Add any pending attributes to the buffer.
	inject_attributes();
	// Inject the character into the buffer.
	buffer.push_back(ch);
}

void Channel::inject(const char* str)
{
	// Add any pending attributes to the buffer.
	inject_attributes();
	// Add the message to the buffer.
	buffer.append(str);
}

void Channel::transmit(bool keep)
{
	// If the buffer is empty, abort transmission.
	if (this->buffer == "") {
		return;
	}

	// Transmit to verbosity-based callbacks as appropriate.
	switch (vrb) {
		// Dispatch on the "quiet" verbosity signal.
		case IOVrb::quiet:
			signal_v_quiet(buffer, cat);
			/* Fall through, so the lower signals get emitted too.
			 * This allows outputs to connect to the HIGHEST
			 * verbosity they will allow, and get the lower verbosity
			 * messages regardless.
			 */
			[[fallthrough]];
		// Dispatch on the "normal" verbosity signal.
		case IOVrb::normal:
			signal_v_normal(buffer, cat);
			[[fallthrough]];
		// Dispatch on the "chatty" verbosity signal.
		case IOVrb::chatty:
			signal_v_chatty(buffer, cat);
			[[fallthrough]];
		// Dispatch on the "TMI" verbosity signal.
		case IOVrb::tmi:
			signal_v_tmi(buffer, cat);
			break;
	}

	// Transmit to category-based callbacks.

	// Dispatch on the "normal" category signal.
	if (flags_check(cat, IOCat::normal)) {
		signal_c_normal(buffer, vrb);
	}
	// Dispatch on the "debug" category signal.
	if (flags_check(cat, IOCat::debug)) {
		signal_c_debug(buffer, vrb);
	}
	// Dispatch on the "warning" category signal.
	if (flags_check(cat, IOCat::warning)) {
		signal_c_warning(buffer, vrb);
	}
	// Dispatch on the "error" category signal.
	if (flags_check(cat, IOCat::error)) {
		signal_c_error(buffer, vrb);
	}
	// Dispatch on the "testing" category signal.
	if (flags_check(cat, IOCat::testing)) {
		signal_c_testing(buffer, vrb);
	}

	// Dispatch the general purpose signals.
	signal_full(buffer, vrb, cat);
	signal_all(buffer);

	// If we are supposed to be echoing...
	if (echo_mode != IOEchoMode::none) {
		// If the verbosity and category is correct...
		if (vrb <= echo_vrb && static_cast<bool>(cat | echo_cat)) {
			// Transmit to standard output using the desired method.
			switch (echo_mode) {
				// If we're supposed to use `printf`...
				case IOEchoMode::printf:
					// For error messages, echo to stderr instead.
					if (flags_check(cat, IOCat::error)) {
						fprintf(stderr, "%s", buffer.c_str());
					}
					// For all other messages, echo to stdout.
					else {
						printf("%s", buffer.c_str());
					}
					break;
				// If we're supposed to use std::cout...
				case IOEchoMode::cout:
					// For error messages, echo to stderr instead.
					if (flags_check(cat, IOCat::error)) {
						std::cerr << buffer.c_str();
					}
					// For all other messages, echo to stdout.
					else {
						std::cout << buffer.c_str();
					}
					break;
				// This case is here for completeness...
				case IOEchoMode::none:
					// ...we should never reach this point!
					assert(false);
					break;
			}
		}
	}

	/* If we aren't flagged to keep formatting,
	 * reset the system in prep for the next message.
	 */
	if (!keep) {
		reset_flags();
	}

	// Clear the message out in preparation for the next.
	clear_buffer();
}

void Channel::inject_attributes()
{
	// If we have no unapplied attributes, abort.
	if (!dirty_attributes) {
		return;
	}
	// Otherwise, inject the attributes into the buffer.
	buffer.append(this->fmt.format_string().c_str());
}

void Channel::reset_attributes()
{
	// Reset the formatting attributes to their defaults.
	this->fmt.reset_attributes();
	// Immediately inject the reset attributes string!
	this->inject(this->fmt.format_string().c_str());
	// We have no pending attributes now.
	this->dirty_attributes = false;
}

void Channel::reset_flags()
{
	// Reset all the flags.
	this->fmt = IOFormat();

	// Reset the verbosity and category.
	vrb = IOVrb::normal;
	cat = IOCat::normal;
}

void Channel::configure_echo(IOEchoMode mode, IOVrb vrb, IOCat cat)
{
	echo_mode = mode;
	echo_vrb = vrb;
	echo_cat = cat;
}

void Channel::shut_up(const IOCat& cat)
{
	this->process_cat = this->process_cat & ~cat;
	if (this->process_cat == IOCat::none) {
		printf("WARNING: All message categories have been turned off!\n");
	}
	// Revalidate parsing.
	parse = maybe;
}

void Channel::shut_up(const IOVrb& vrb)
{
	// Set the processing verbosity.
	process_vrb = vrb;
	// Revalidate parsing.
	parse = maybe;
}

void Channel::speak_up(const IOCat& cat)
{
	// Allow the category through by turning on its bit.
	this->process_cat = this->process_cat | cat;
	// Revalidate parsing.
	parse = maybe;
}

void Channel::speak_up(const IOVrb& vrb)
{
	// Allow verbosity through by turning on its bit.
	if (this->process_vrb < vrb) {
		this->process_vrb = vrb;
		// Revalidate parsing.
		parse = maybe;
	}
}

void Channel::speak_up()
{
	process_vrb = IOVrb::tmi;
	process_cat = IOCat::all;
	// Revalidate parsing.
	parse = maybe;
}
