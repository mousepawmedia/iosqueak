/** Bitfield [IOSqueak]
 * Version: 2.0.0
 *
 * Bitfield utility functions.
 *
 * TODO: Relocate this to PawLIB.
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

#ifndef IOSQUEAK_BITFIELD_HPP
#define IOSQUEAK_BITFIELD_HPP

/** Convert a struct to a bitfield by generating the bitwise operators.
 *  \param T: the struct (type) to generate operators for.
 */
#define MAKE_BITFIELD(T)                                            \
	inline T operator&(const T& lhs, const T& rhs)                  \
	{                                                               \
		return flags_and(lhs, rhs);                                 \
	}                                                               \
                                                                    \
	inline T operator|(const T& lhs, const T& rhs)                  \
	{                                                               \
		return flags_or(lhs, rhs);                                  \
	}                                                               \
                                                                    \
	inline T operator^(const T& lhs, const T& rhs)                  \
	{                                                               \
		return flags_xor(lhs, rhs);                                 \
	}                                                               \
                                                                    \
	inline T operator~(const T& lhs) { return flags_twiddle(lhs); } \
	asm("")  // swallow the semicolon

/** Perform bitwise AND on a bitfield.
 * \param lhs: the left operand
 * \param rhs: the right operand
 * \return the result of the bitwise operation as a bitfield.
 */
template<typename T>
T flags_and(const T& lhs, const T& rhs)
{
	return static_cast<T>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

/** Perform bitwise OR on a bitfield.
 * \param lhs: the left operand
 * \param rhs: the right operand
 * \return the result of the bitwise operation as a bitfield.
 */
template<typename T>
T flags_or(const T& lhs, const T& rhs)
{
	return static_cast<T>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

/** Perform bitwise XOR on a bitfield.
 * \param lhs: the left operand
 * \param rhs: the right operand
 * \return the result of the bitwise operation as a bitfield.
 */
template<typename T>
T flags_xor(const T& lhs, const T& rhs)
{
	return static_cast<T>(static_cast<int>(lhs) ^ static_cast<int>(rhs));
}

/** Perform bitwise invert on a bitfield.
 * \param rhs: the operand
 * \return the result of the bitwise operation as a bitfield.
 */
template<typename T>
T flags_twiddle(const T& rhs)
{
	return static_cast<T>(~static_cast<int>(rhs));
}

/** Check if a value is found in a bitfield
 * \param field: the bitfield to check
 * \param value: the value to check for (must be same type as field)
 * \return true if flag is found in bitfield, else false
 */
template<typename T>
bool flags_check(const T& field, const T& value)
{
	return static_cast<bool>(field & value);
}

#endif
