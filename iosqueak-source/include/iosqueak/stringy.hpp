/** Utility Functions [IOSqueak]
 * Version: 1.0
 *
 * NOTE: Rename/rework this class!
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

#ifndef IOSQUEAK_UTILS_HPP
#define IOSQUEAK_UTILS_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
// Needed for stdsplit
#include <string>

namespace stringy
{
/** Efficiently split a std::string by tokens.
 * \param the string to split
 * \param the character or string to split by
 * \param the vector to store the tokens in
 */
inline void stdsplit(std::string str,
					 std::string splitBy,
					 std::vector<std::string>& tokens)
{
	/* Store the original string in the array, so we can loop the rest
	 * of the algorithm. */
	tokens.push_back(str);

	// Store the size of what we're splicing out.
	size_t splitLen = splitBy.size();
	// Create a string for temporarily storing the fragment we're processing.
	std::string frag;

	// Loop infinitely - break is internal.
	while (true) {
		// Store the split index in a 'size_t' (unsigned integer) type.
		size_t splitAt;

		/* Store the last string in the vector, which is the only logical
		 * candidate for processing. */
		frag = tokens.back();
		/* The index where the split is. */
		splitAt = frag.find(splitBy);
		// If we didn't find a new split point...
		if (splitAt == std::string::npos) {
			// Break the loop and (implicitly) return.
			break;
		}
		/* Put everything from the left side of the split where the string
		 * being processed used to be. */
		tokens.back() = frag.substr(0, splitAt);
		/* Push everything from the right side of the split to the next empty
		 * index in the vector. */
		tokens.push_back(frag.substr(splitAt + splitLen,
									 frag.size() - (splitAt + splitLen)));
	}
}

/** Reverse a C-string. Algorithm from
 * http://stackoverflow.com/a/784567/472647
 * \param the C-string to reverse.
 * \param the reversed C-string
 */
inline char* strrev(char* str)
{
	if (str != NULL) {
		char* src;
		size_t len;

		len = strlen(str);
		if (len > 1) {
			char* dst;
			src = str;
			dst = src + len - 1;
			while (src < dst) {
				char tmp = *src;
				*src++ = *dst;
				*dst-- = tmp;
			}
		}
	}
	return str;
}
// TODO: Move this to onestring!

template<typename T>
inline void arev(T* arr, size_t start, size_t end)
{
	T tmp;
	while (start < end) {
		memcpy(&tmp, (arr + end), sizeof(T));
		memcpy((arr + end), (arr + start), sizeof(T));
		memcpy((arr + start), &tmp, sizeof(T));
		++start;
		--end;
	}
}

}  // namespace stringy

#endif  // IOSQUEAK_UTILS_HPP
