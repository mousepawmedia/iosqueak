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
#include <string>

// We use C's classes often.
#include <cstdio>

// Signals and callbacks.
#include "eventpp/callbacklist.h"

// For tril data type
#include "arctic-tern/tril.hpp"
#include "iosqueak/ioctrl.hpp"
#include "iosqueak/ioformat.hpp"
#include "iosqueak/stringify.hpp"

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
	bool dirty_attributes;

	/** Determines whether the verbosity and category match parsing rules.
	 * \return true if we can definitely parse
	 */
	bool can_parse();

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
	void flush();

	/** Move the cursor given the command.
	 * \param rhs: the cursor command
	 */
	void move_cursor(const IOCursor& rhs);

	/** Insert a single character without need for null terminator.
	 * \param ch: the character to insert
	 */
	void inject(char ch);

	/** Insert a C string into the output stream. Automatically applies
	 * unapplied attributes before inserting text.
	 * \param str: the C string to insert
	 */
	void inject(const char* str);

	/** Insert a standard string into the output stream. Automatically
	 * applies unapplied attributes before inserting text.
	 * \param str: the std::string to insert
	 */
	inline void inject(const std::string& str) { inject(str.c_str()); }

	/** Transmit the current pending output stream and reset in
	 * preparation for the next message.
	 * \param whether to retain the flags
	 */
	void transmit(bool keep = false);

	void clear_buffer() { buffer.clear(); }

	/** Injects the current attribute string into the buffer. */
	void inject_attributes();

	/** Reset all attributes, and inject the reset attribute string
	 * into the buffer.*/
	void reset_attributes();

	/**Reset all flags.*/
	void reset_flags();

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

		if (flags_check(rhs, IOCtrl::r)) {
			can_parse() ? inject("\r") : inject("");
		}

		if (flags_check(rhs, IOCtrl::n)) {
			can_parse() ? inject("\n") : inject("");
		}

		if (flags_check(rhs, IOCtrl::clear)) {
			reset_attributes();
			keep = false;
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
						IOCat cat = IOCat::all);

	/** Suppress a category from broadcasting at all.
	 * \param the category to suppress
	 */
	void shut_up(const IOCat& cat);

	/** Suppress a verbosity from broadcasting at all.
	 * All verbosities greater than the one specified will be silenced.
	 * \param the verbosity to suppress
	 */
	void shut_up(const IOVrb& vrb = IOVrb::normal);

	/** Permit a category to broadcast.
	 * \param the category to broadcast
	 */
	void speak_up(const IOCat& cat);

	/** Permit a verbosity to broadcast.
	 * All verbosities greater than the one specified will also broadcast.
	 * \param the verbosity to broadcast
	 */
	void speak_up(const IOVrb& vrb);

	/** Permit all messages to broadcast.
	 * Sets verbosity and category to permit all messages.
	 */
	void speak_up();

	~Channel() = default;
};

/// Global instance of Channel.
static inline Channel channel = Channel();

#endif
