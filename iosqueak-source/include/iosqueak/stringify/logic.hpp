/** Stringify: Logic [IOSqueak]
 *  Version 1.0
 *
 *  Convert any data type to strings.
 *
 * Author(s): Jason C. McDonald, Scott Taylor
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

#ifndef IOSQUEAK_STRINGIFY_LOGIC_HPP
#define IOSQUEAK_STRINGIFY_LOGIC_HPP

#include <string>

#include "iosqueak/ioformat.hpp"

/**Count the number of characters necessary to represent a boolean
 * as a string. Does not count the null terminator.
 * \param the boolean to convert to a string
 * \param the format to represent the boolean in, default lowercase word
 * \return the calculated string length */
inline size_t lengthify_boolean(
	const bool& val, const IOFormatBoolStyle& fmt = IOFormatBoolStyle::lower)
{
	switch (fmt) {
		// TRUE has four letters, FALSE has five...
		case IOFormatBoolStyle::lower:
		case IOFormatBoolStyle::upper:
		case IOFormatBoolStyle::caps:
			return val ? 4 : 5;
		// 1 and 0 each are one letter...
		case IOFormatBoolStyle::numeral:
			return 1;
		// PASS and FAIL have four letters each...
		case IOFormatBoolStyle::test:
			return 4;
		// YEA and NAY have three letters each...
		case IOFormatBoolStyle::scott:
			return 3;
	}
	return 0;
}

inline std::string stringify_boolean(
	const bool& val, const IOFormatBoolStyle& fmt = IOFormatBoolStyle::lower)
{
	switch (fmt) {
		case IOFormatBoolStyle::lower:
			return val ? "true" : "false";
		case IOFormatBoolStyle::upper:
			return val ? "True" : "False";
		case IOFormatBoolStyle::caps:
			return val ? "TRUE" : "FALSE";
		case IOFormatBoolStyle::numeral:
			return val ? "1" : "0";
		case IOFormatBoolStyle::test:
			return val ? "PASS" : "FAIL";
		case IOFormatBoolStyle::scott:
			return val ? "yea" : "nay";
	}
	return "";
}

#endif
