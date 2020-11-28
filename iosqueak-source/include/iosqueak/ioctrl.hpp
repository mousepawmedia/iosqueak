/** IOCtrl [IOSqueak]
 * Version: 2.0.0
 *
 * Control flags for Channel.
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

#ifndef IOSQUEAK_IOCTRL_HPP
#define IOSQUEAK_IOCTRL_HPP

#include "iosqueak/utilities/bitfield.hpp"

/// The category of the message.
enum class IOCat {
	/// No category. Internal use only; no correlating signal.
	none = 0,
	/// The default value - anything that doesn't fit elsewhere.
	normal = 1,
	/// Warnings, but not necessarily errors.
	warning = 2,
	/// Error messages.
	error = 4,
	/// Debug messages, such as variable outputs.
	debug = 8,
	/// Testing messages that we may want shut off during benchmarking.
	testing = 16,
	/// All message categories. Internal use only; no correlating signal.
	all = 31
};

/// IOCat should be interpreted as a bitfield.
MAKE_BITFIELD(IOCat);

/// Controls the output of the Channel.
enum class IOCtrl {
	// BITFIELD: Be cautious of the numbers assigned here!

	/// Send message, clearing the buffer.
	send = 1,
	/// Clear buffer after sending.
	clear = 2,
	/// Print carriage return.
	r = 4,
	/// Print newline.
	n = 8,
	/// Flush standard output.
	flush = 16,

	// The following options are combinations of the above, for convenience.

	/// Send with carriage return (\r), keep formatting
	sendc = 1 | 4 | 16,
	/// Send with line feed (\n), keep formatting
	sendl = 1 | 8 | 16,
	/// Send, clear formatting
	end = 1 | 2,
	/// End with carriage return (\r), clear formatting
	endc = 1 | 2 | 4 | 16,
	/// End with line feed (\n), clear formatting
	endl = 1 | 2 | 8 | 16,
};

/// IOCtrl should be treated as a bitfield.
MAKE_BITFIELD(IOCtrl);

// Basic cursor movement.
enum class IOCursor {
	// Move cursor left ('\[1D]')
	left,
	// Move cursor right ('\[1C]')
	right
};

/// Controls whether (and how) a channel outputs to the standard streams.
enum class IOEchoMode {
	/// Don't automatically output messages via the stdout/stderr echo.
	none = 0,
	/// Output messages to stdout via C-style `printf`.
	printf = 1,
	/// Output messages to stdout via C++-style `std::cout`.
	cout = 2
	// TODO: Expand to allow turning on/off ONLY cerr or ONLY cout, etc.
};

/**Indicate how many bytes to read from any pointer that isn't
 * recognized explicitly by channel, including void pointers.
 * This will NOT override the memory dump read size of existing types.
 * CAUTION: Misuse can cause SEGFAULT or other memory errors.
 * \param bytes: the number of bytes to read
 * */
struct IOMemReadSize {
	explicit IOMemReadSize(unsigned int bytes) : readsize(bytes) {}
	size_t readsize = 1;

	void operator=(size_t rhs) { readsize = rhs; }
};

/// The level of verbosity necessary for the message to display.
enum class IOVrb {
	/**Only essential messages and errors. For normal end-use.
	Shipping default.*/
	quiet = 0,
	/**Common messages and errors. For common and normal end-user
	testing.*/
	normal = 1,
	/**Most messages and errors. For detailed testing and
	debugging.*/
	chatty = 2,
	/**Absolutely everything. For intense testing, detailed
	debugging, and driving the developers crazy.*/
	tmi = 3
};

#endif
