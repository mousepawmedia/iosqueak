/** Channel [IOSqueak]
 * Version: 2.0.0
 *
 * Allows managed, custom output to any console or terminal.
 *
 * Prints out (virtually) any value in a human-readable form that is
 * appropriate to the data type. For example, a boolean prints as 'true' or
 * 'false', and a pointer prints as a hexadecimal memory address.
 *
 * Messages can be routed based on category and verbosity (priority) to
 * the standard output/standard error streams, or to any custom function
 * via callback.
 *
 * Fully supports all ANSI formatting, so you can make text bold, blue, and
 * flashing (or whatever) without having to memorize ANSI control codes.
 *
 * In other words, it prints things nicely!
 *
 * Author: Jason C. McDonald
 */

/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2016-2020 MousePaw Media.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * CONTRIBUTING
 * See https://www.mousepawmedia.com/developers for information
 * on how to contribute to our projects.
 */

#ifndef IOSQUEAK_CHANNEL_HPP
#define IOSQUEAK_CHANNEL_HPP

#include <assert.h>

// Needed for output sources.
#include <iostream>
#include <stdio.h>

// Needed for the `intptr_t` type
#include <cstdint>

// Needed for `ceil()`
#include <cmath>

// Bitset
#include <bitset>

// Needed for checking types.
#include <type_traits>
#include <typeinfo>

// Needed for handling passed-in exceptions.
#include <exception>

// Signals and callbacks.
#include "eventpp/callbacklist.h"

/*We are only using std::string and std::queue temporarily.
These need to be swapped out for pawlib alternatives ASAP.*/
#include <string>

// We use C's classes often.
#include <cstdio>

#include "iosqueak/ioctrl.hpp"
#include "iosqueak/ioformat.hpp"
#include "iosqueak/stringify.hpp"
#include "pawlib/core_types.hpp"

class Channel
{
protected:
	std::string buffer;

	/// Which categories are permitted.
	IOCat process_cat;
	/// The maximum verbosity to permit.
	IOVrb process_vrb;

	// Which method should be used for Channel's default standard stream echo?
	IOEchoMode echo_mode;
	// The categories to echo.
	IOCat echo_cat;
	// The maximum verbosity to echo.
	IOVrb echo_vrb;

	// Message attributes.
	IOFormat fmt;
	IOVrb vrb;
	IOCat cat;

	// Message parsable?
	tril parse;

	/// Dirty flag raised when attributes are changed and not yet applied.
	bool dirty_attributes = false;

	/** Determines whether the verbosity and category match parsing rules.
	 * \return true if we can definitely parse
	 */
	bool can_parse()
	{
		// If we aren't sure about the parsing condition...
		if (~parse) {
			/* Proceed if the verbosity is in range
			 * and the category is set to parse. */
			parse = ((vrb <= process_vrb) && flags_check(process_cat, cat))
						? true
						: false;
		}
		return parse;
	}

	/** Process a formatting flag.
	 * \param val: the flag to process
	 * \return true if processed
	 */
	template<typename T>
	bool process_fmt(T val)
	{
		// If we are blocked from parsing, abort.
		if (!can_parse()) {
			return false;
		}

		// Inject the formatting flag into the IOFormat
		this->fmt << val;
		// Require attributes to be reparsed.
		dirty_attributes = true;
		// Report processing was successful.
		return true;
	}

	/** Flush the standard output. */
	void flush()
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

	/** Move the cursor given the command.
	 * \param rhs: the cursor command
	 */
	void move_cursor(const IOCursor& rhs)
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

	/** Insert a single character without need for null terminator.
	 * \param ch: the character to insert
	 */
	void inject(char ch)
	{
		// Add any pending attributes to the buffer.
		inject_attributes();
		// Inject the character into the buffer.
		buffer.push_back(ch);
	}

	/** Insert a C string into the output stream. Automatically applies
	 * unapplied attributes before inserting text.
	 * \param str: the C string to insert
	 */
	void inject(const char* str)
	{
		// Add any pending attributes to the buffer.
		inject_attributes();
		// Add the message to the buffer.
		buffer.append(str);
	}

	/** Insert a standard string into the output stream. Automatically
	 * applies unapplied attributes before inserting text.
	 * \param str: the std::string to insert
	 */
	inline void inject(const std::string& str) { inject(str.c_str()); }

	/** Transmit the current pending output stream and reset in
	 * preparation for the next message.
	 * \param whether to retain the flags
	 */
	void transmit(bool keep = false)
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

	void clear_buffer() { buffer.clear(); }

	/** Injects the current attribute string into the buffer. */
	void inject_attributes()
	{
		// If we have no unapplied attributes, abort.
		if (!dirty_attributes) {
			return;
		}
		// Otherwise, inject the attributes into the buffer.
		buffer.append(this->fmt.format_string().c_str());
	}

	/** Reset all attributes, and inject the reset attribute string
	 * into the buffer.*/
	void reset_attributes()
	{
		// Reset the formatting attributes to their defaults.
		this->fmt.reset_attributes();
		// Immediately inject the reset attributes string!
		this->inject(this->fmt.format_string().c_str());
		// We have no pending attributes now.
		this->dirty_attributes = false;
	}

	/**Reset all flags.*/
	void reset_flags()
	{
		// Reset all the flags.
		this->fmt = IOFormat();

		// Reset the verbosity and category.
		vrb = IOVrb::normal;
		cat = IOCat::normal;
	}

public:
	Channel()
	: buffer(""), process_cat(IOCat::all), process_vrb(IOVrb::tmi),
	  echo_mode(IOEchoMode::cout), echo_cat(IOCat::all), echo_vrb(IOVrb::tmi),
	  fmt(IOFormat()), vrb(IOVrb::normal), cat(IOCat::normal), parse(maybe),
	  dirty_attributes(false)
	{
	}

	/// Signal for categories.
	typedef eventpp::CallbackList<void(std::string, IOCat)> IOSignalCat;

	/** Eventpp signal (callback list) for verbosities. */
	typedef eventpp::CallbackList<void(std::string, IOVrb)> IOSignalVrb;

	/** Eventpp signal (callback list) for everything,
	 * transmitting the message, the verbosity, and the category. */
	typedef eventpp::CallbackList<void(std::string, IOVrb, IOCat)> IOSignalFull;

	/** Eventpp signal (callback list) for everything,
	 * transmitting only the message. */
	typedef eventpp::CallbackList<void(std::string)> IOSignalAll;

	/* NOTE: In the examples below, the verbosity-related signals must
	 * transmit what category the message is (since verbosity is
	 * inherent and assumed). The inverse is true of category-related
	 * signals. */

	/** Emitted when a message with verbosity 0 (quiet) is broadcast.
	 * Callback must be of form 'void callback(string,
	 * IOCat){}'
	 */
	IOSignalCat signal_v_quiet;

	/** Emitted when a message with verbosity <= 1 (normal) is broadcast.
	 * Callback must be of form 'void callback(string,
	 * IOCat){}'
	 */
	IOSignalCat signal_v_normal;

	/** Emitted when a message with verbosity <=2 (chatty) is broadcast.
	 * Callback must be of form 'void callback(string,
	 * IOCat){}'
	 */
	IOSignalCat signal_v_chatty;

	/** Emitted when a message with verbosity <=3 (tmi) is broadcast.
	 * Callback must be of form 'void callback(string,
	 * IOCat){}'
	 */
	IOSignalCat signal_v_tmi;

	/** Emitted when a message with category "normal" is broadcast.
	 * Callback must be of form 'void callback(string,
	 * IOVrb){}'
	 */
	IOSignalVrb signal_c_normal;

	/** Emitted when a message with category "warning" is broadcast.
	 * Callback must be of form 'void callback(string,
	 * IOVrb){}'
	 */
	IOSignalVrb signal_c_warning;

	/** Emitted when a message with category "error" is broadcast.
	 * Callback must be of form 'void callback(string,
	 * IOVrb){}'
	 */
	IOSignalVrb signal_c_error;

	/** Emitted when a message with category "debug" is broadcast.
	 * Callback must be of form 'void callback(string,
	 * IOVrb){}'
	 */
	IOSignalVrb signal_c_debug;

	/** Emitted when a message with category "testing" is broadcast.
	 * Callback must be of form 'void callback(string,
	 * IOVrb){}'
	 */
	IOSignalVrb signal_c_testing;

	/** Emitted when any message is broadcast.
	 * Callback must be of form 'void callback(string,
	 * IOVrb, IOCat){}'
	 */
	IOSignalFull signal_full;

	/** Emitted when any message is broadcast, but only transmits the
	 * message, and omits the verbosity and category.
	 * Callback must be of form 'void callback(string){}'
	 */
	IOSignalAll signal_all;

	// Process formatting flags.
	Channel& operator<<(const IOFormatBase& rhs)
	{
		process_fmt(rhs);
		return *this;
	}
	Channel& operator<<(const IOFormatBaseNotation& rhs)
	{
		process_fmt(rhs);
		return *this;
	}
	Channel& operator<<(const IOFormalBoolStyle& rhs)
	{
		process_fmt(rhs);
		return *this;
	}
	Channel& operator<<(const IOFormatCharValue& rhs)
	{
		process_fmt(rhs);
		return *this;
	}
	Channel& operator<<(const IOFormatDecimalPlaces& rhs)
	{
		process_fmt(rhs);
		return *this;
	}
	Channel& operator<<(const IOFormatMemSep& rhs)
	{
		process_fmt(rhs);
		return *this;
	}
	Channel& operator<<(const IOFormatNumCase& rhs)
	{
		process_fmt(rhs);
		return *this;
	}
	Channel& operator<<(const IOFormatPtr& rhs)
	{
		process_fmt(rhs);
		return *this;
	}
	Channel& operator<<(const IOFormatSciNotation& rhs)
	{
		process_fmt(rhs);
		return *this;
	}
	Channel& operator<<(const IOFormatSign& rhs)
	{
		process_fmt(rhs);
		return *this;
	}
	Channel& operator<<(const IOFormatStandard& rhs)
	{
		process_fmt(rhs);
		return *this;
	}
	Channel& operator<<(const IOFormatTextAttr& rhs)
	{
		process_fmt(rhs);
		return *this;
	}
	Channel& operator<<(const IOFormatTextBG& rhs)
	{
		process_fmt(rhs);
		return *this;
	}
	Channel& operator<<(const IOFormatTextFG& rhs)
	{
		process_fmt(rhs);
		return *this;
	}

	// Set message category.
	Channel& operator<<(const IOCat& rhs)
	{
		// Set the category and force revalidation of parsing.
		this->cat = rhs;
		this->parse = maybe;
		return *this;
	}

	// Set message verbosity.
	Channel& operator<<(const IOVrb& rhs)
	{
		// Set the verbosity and force the revalidation of parsing.
		this->vrb = rhs;
		this->parse = maybe;
		return *this;
	}

	// Control channel.
	Channel& operator<<(const IOCtrl& rhs)
	{
		/* We'll need to track whether we're supposed to ask the transmission
		 * to clear the flags. Doing it ourselves in this function
		 * was the cause of T1081. */
		bool keep = true;

		if (flags_check(rhs, IOCtrl::clear)) {
			reset_attributes();
			keep = false;
		}

		if (flags_check(rhs, IOCtrl::r)) {
			can_parse() ? inject("\r") : inject("");
		}

		if (flags_check(rhs, IOCtrl::n)) {
			can_parse() ? inject("\n") : inject("");
		}

		if (flags_check(rhs, IOCtrl::send)) {
			transmit(keep);
		}

		if (flags_check(rhs, IOCtrl::flush)) {
			flush();
		}

		return *this;
	}

	// Inject c-string into buffer.
	Channel& operator<<(const char* rhs)
	{
		if (!can_parse()) {
			return *this;
		}

		inject(rhs);

		return *this;
	}

	// Inject std::string into buffer.
	Channel& operator<<(const std::string& rhs)
	{
		if (!can_parse()) {
			return *this;
		}

		inject(rhs.c_str());

		return *this;
	}

	// Inject anything else, using stringify.
	template<typename T>
	Channel& operator<<(const T& rhs)
	{
		if (!can_parse()) {
			return *this;
		}

		inject(stringify(rhs, this->fmt));

		return *this;
	}

	/** Configure if/when channel echoes to the standard output.
	 * \param mode: the echo mode (typically cout or fstream)
	 * \param vrb: the maximum verbosity to echo.
	 * \param cat: the categories to echo.
	 */
	void configure_echo(IOEchoMode mode,
						IOVrb vrb = IOVrb::tmi,
						IOCat cat = IOCat::all)
	{
		echo_mode = mode;
		echo_vrb = vrb;
		echo_cat = cat;
	}

	/** Suppress a category from broadcasting at all.
	 * \param the category to suppress
	 */
	void shut_up(const IOCat& cat)
	{
		this->process_cat = this->process_cat & ~cat;
		if (this->process_cat == IOCat::none) {
			printf("WARNING: All message categories have been turned off!\n");
		}
		// Revalidate parsing.
		parse = maybe;
	}

	/** Suppress a verbosity from broadcasting at all.
	 * All verbosities greater than the one specified will be silenced.
	 * \param the verbosity to suppress
	 */
	void shut_up(const IOVrb& vrb = IOVrb::normal)
	{
		// Set the processing verbosity.
		process_vrb = vrb;
		// Revalidate parsing.
		parse = maybe;
	}

	/** Permit a category to broadcast.
	 * \param the category to broadcast
	 */
	void speak_up(const IOCat& cat)
	{
		// Allow the category through by turning on its bit.
		this->process_cat = this->process_cat | cat;
		// Revalidate parsing.
		parse = maybe;
	}

	/** Permit a verbosity to broadcast.
	 * All verbosities greater than the one specified will also broadcast.
	 * \param the verbosity to broadcast
	 */
	void speak_up(const IOVrb& vrb)
	{
		// Allow verbosity through by turning on its bit.
		if (this->process_vrb < vrb) {
			this->process_vrb = vrb;
			// Revalidate parsing.
			parse = maybe;
		}
	}

	/** Permit all messages to broadcast.
	 * Sets verbosity and category to permit all messages.
	 */
	void speak_up()
	{
		process_vrb = IOVrb::tmi;
		process_cat = IOCat::all;
		// Revalidate parsing.
		parse = maybe;
	}

	~Channel() = default;
};

/// Global instance of Channel.
static inline Channel channel = Channel();

#endif
