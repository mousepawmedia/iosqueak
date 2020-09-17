/** Channel [IOSqueak]
 * Version: 2.0.0
 *
 * Allows managed, custom output to any console or terminal.
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

// Signals and callbacks.
#include <string>

#include "eventpp/callbacklist.h"

// We use C's classes often.
#include <cstdio>

#include "iosqueak/ioformat.hpp"
#include "iosqueak/stringify.hpp"
#include "pawlib/core_types.hpp"

class Channel
{
protected:
	std::string msg;

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

	/// The string containing the formatting codes.
	std::string format_str;

	// Message parsable?
	tril parse;

	/// Dirty flag raised when attributes are changed and not yet applied.
	bool dirty_attributes = false;

	/** Returns whether the verbosity and category match parsing rules. */
	bool can_parse()
	{
		// If we aren't sure about the parsing condition...
		if (~parse)
		{
			/* Proceed if the verbosity is in range
			 * and the category is set to parse. */
			parse =
				((vrb <= process_vrb) && static_cast<bool>(process_cat & cat))
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
		if (!can_parse())
		{
			return false;
		}

		this->fmt << val;
		dirty_attributes = true;
		return true;
	}

	/** Flush the standard output. */
	void flush()
	{
		/* TODO: For non-standard outputs, this should attach a bytecode to
		 * tell external outputs to flush.
		 */

		// Flush is essential for progress-style outputs (after \r and no \n)
		switch (echo_mode)
		{
			case IOEchoMode::cout:
				std::cout << std::flush;
				break;
			case IOEchoMode::printf:
				fflush(stdout);
				break;
			case IOEchoMode::none:
				break;
		}
	}

	/** Move the cursor given the command.
	 * \param the cursor command
	 */
	void move_cursor(const IOCursor& rhs)
	{
		// TODO: Migrate to ioformat.hpp
		if (this->fmt.standard() == IOFormatStandard::ansi)
		{
			switch (rhs)
			{
				case IOCursor::left:
					// NOTE: Watch this. \x1B is allegedly equal to \e, check it.
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
	 * \param the character to insert
	 */
	void inject(char ch)
	{
		// If we just applied attributes, push them now.
		if (apply_attributes())
		{
			inject(this->format_str.c_str());
		}
		msg.push_back(ch);
	}

	/** Insert a C string into the output stream. Automatically applies
	 * unapplied attributes before inserting text.
	 * \param the C string to insert
	 * \param whether the call was recursive. (Internal use only!)
	 */
	void inject(const char* str, bool recursive = false)
	{
		// If we just applied attributes, push them now.
		if (!recursive && apply_attributes())
		{
			inject(format_str.c_str(), true);
		}

		// Append to the message.
		msg.append(str);
	}

	void inject(const std::string& str)
	{
		inject(str.c_str());
	}

	/** Transmit the current pending output stream and reset in
	 * preparation for the next message.
	 * \param whether to retain the flags
	 */
	void transmit(bool keep = false)
	{
		if (this->msg != "")
		{
			switch (vrb)
			{
				case IOVrb::quiet:
					// Dispatch on the "quiet" verbosity signal.
					signal_v_quiet(msg, cat);
					/* Fall through, so the lower signals get emitted too.
					 * This allows outputs to connect to the HIGHEST
					 * verbosity they will allow, and get the lower verbosity
					 * messages regardless.
					 */
					[[fallthrough]];
				case IOVrb::normal:
					// Dispatch on the "normal" verbosity signal.
					signal_v_normal(msg, cat);
					[[fallthrough]];
				case IOVrb::chatty:
					// Dispatch on the "chatty" verbosity signal.
					signal_v_chatty(msg, cat);
					[[fallthrough]];
				case IOVrb::tmi:
					// Dispatch on the "TMI" verbosity signal.
					signal_v_tmi(msg, cat);
					break;
			}

			if (static_cast<bool>(cat & IOCat::normal))
			{
				// Dispatch on the "normal" category signal.
				signal_c_normal(msg, vrb);
			}
			if (static_cast<bool>(cat & IOCat::debug))
			{
				// Dispatch on the "debug" category signal.
				signal_c_debug(msg, vrb);
			}
			if (static_cast<bool>(cat & IOCat::warning))
			{
				// Dispatch on the "warning" category signal.
				signal_c_warning(msg, vrb);
			}
			if (static_cast<bool>(cat & IOCat::error))
			{
				// Dispatch on the "error" category signal.
				signal_c_error(msg, vrb);
			}
			if (static_cast<bool>(cat & IOCat::testing))
			{
				// Dispatch on the "testing" category signal.
				signal_c_testing(msg, vrb);
			}

			// Dispatch the general purpose signals.
			signal_full(msg, vrb, cat);
			signal_all(msg);

			// If we are supposed to be echoing...
			if (echo_mode != IOEchoMode::none)
			{
				// If the verbosity and category is correct...
				if (vrb <= echo_vrb && static_cast<bool>(cat | echo_cat))
				{
					switch (echo_mode)
					{
						// If we're supposed to use `printf`...
						case IOEchoMode::printf:
							// For error messages, echo to stderr instead.
							if (static_cast<bool>(cat & IOCat::error))
							{
								fprintf(stderr, "%s", msg.c_str());
							}
							// For all other messages, echo to stdout.
							else
							{
								printf("%s", msg.c_str());
							}
							break;
						// If we're supposed to use std::cout...
						case IOEchoMode::cout:
							// For error messages, echo to stderr instead.
							if (static_cast<bool>(cat & IOCat::error))
							{
								std::cerr << msg.c_str();
							}
							// For all other messages, echo to stdout.
							else
							{
								std::cout << msg.c_str();
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
			if (!keep)
			{
				reset_flags();
			}

			// Clear the message out in preparation for the next.
			clear_msg();
		}
	}

	bool apply_attributes()
	{
		// If we have unapplied attributes, create attribute format string.
		if (!dirty_attributes) { return false; }

		this->format_str = this->fmt.format_string();

		return true;
	}

	void clear_msg()
	{
		msg.clear();
	}

	/**Reset all attributes.*/
	void reset_attributes()
	{
		this->fmt.reset_attributes();
		this->dirty_attributes = false;
		/* We must leave calling `apply_attributes()` to `inject()`,
		 * otherwise the reset attributes will never get injected
		 * into the broadcast stream.
		 */
	}

	/**Reset all flags.*/
	void reset_flags()
	{
		// Reset all the flags.
		this->fmt = IOFormat();

		// We reset the verbosity and category.
		vrb = IOVrb::normal;
		cat = IOCat::normal;
	}

public:
	Channel()
	: msg(""), process_cat(IOCat::all), process_vrb(IOVrb::tmi),
	  echo_mode(IOEchoMode::cout), echo_cat(IOCat::all), echo_vrb(IOVrb::tmi),
	  fmt(IOFormat()), vrb(IOVrb::normal), cat(IOCat::normal),
	  format_str(""), parse(maybe),
	  dirty_attributes(false)
	{}

	/// Signal for categories.
	typedef eventpp::CallbackList<void(std::string, IOCat)> IOSignalCat;

	/// Signal for verbosities.
	typedef eventpp::CallbackList<void(std::string, IOVrb)> IOSignalVrb;

	/// Signal for everything.
	typedef eventpp::CallbackList<void(std::string, IOVrb, IOCat)> IOSignalFull;

	/// Signal for everything, sans verbosity and category.
	typedef eventpp::CallbackList<void(std::string)> IOSignalAll;

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
	Channel& operator<<(const IOFormatBoolStyle& rhs)
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

	Channel& operator<<(const IOCat& rhs)
	{
		// Set the category and force revalidation of parsing.
		this->cat = rhs;
		this->parse = maybe;
		return *this;
	}

	Channel& operator<<(const IOVrb& rhs)
	{
		// Set the verbosity and force the revalidation of parsing.
		this->vrb = rhs;
		this->parse = maybe;
		return *this;
	}

	Channel& operator<<(const IOCtrl& rhs)
	{
		/* We'll need to track whether we're supposed to ask the transmission
		 * to clear the flags. Doing it ourselves in this function
		 * was the cause of T1081. */
		bool keep = true;

		if (flags_check(rhs, IOCtrl::clear))
		{
			reset_attributes();
			keep = false;
		}

		if (flags_check(rhs, IOCtrl::r))
		{
			can_parse() ? inject("\r") : inject("");
		}

		if (flags_check(rhs, IOCtrl::n))
		{
			can_parse() ? inject("\n") : inject("");
		}

		if (flags_check(rhs, IOCtrl::send))
		{
			transmit(keep);
		}

		if (flags_check(rhs, IOCtrl::flush))
		{
			flush();
		}

		return *this;
	}

	template<typename T>
	Channel& operator<<(const T& rhs)
	{
		if(!can_parse()) { return *this; }

		inject(stringify(rhs));

		return *this;
	}

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
		if (this->process_cat == IOCat::none)
		{
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
		if (this->process_vrb < vrb)
		{
			this->process_vrb = vrb;
			// Revalidate parsing.
			parse = maybe;
		}
	}

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
