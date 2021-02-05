/** Stringify: Utilities [IOSqueak]
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

#ifndef IOSQUEAK_STRINGIFY_UTILITIES_HPP
#define IOSQUEAK_STRINGIFY_UTILITIES_HPP

#include <string>
#include <typeindex>

#include "iosqueak/ioformat.hpp"
#include "iosqueak/tools/memlens.hpp"

/**Reverse a string, creating a new copy.
 * \param the string to reverse
 * \param the reversed string */
inline std::string reversify(const std::string& str)
{
	// If the string has less than 2 characters, return as-is.
	if (str.length() < 2) {
		return str;
	}

	// I'll read from the end of the input string to the beginning.
	size_t target = str.length();

	// Create the output string with enough space pre-reserved.
	std::string reversed = std::string();
	reversed.reserve(target);

	/* Append characters from the input string to the output string,
	 * reading from the last character to the first. */
	do {
		reversed += str[target - 1];
	} while (--target > 0);

	return reversed;
}

#endif
