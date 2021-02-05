/** Stringify: Memory [IOSqueak]
 *  Version 1.0
 *
 *  Convert pointers to strings in a variety of ways.
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

#ifndef IOSQUEAK_STRINGIFY_MEMORY_HPP
#define IOSQUEAK_STRINGIFY_MEMORY_HPP

#include <math.h>
#include <stdexcept>
#include <string>

#include "iosqueak/ioformat.hpp"
#include "iosqueak/stringify/numbers.hpp"
#include "iosqueak/stringify/types.hpp"
#include "iosqueak/stringify/utilities.hpp"
#include "iosqueak/tools/memlens.hpp"
#include "iosqueak/utilities/bitfield.hpp"

/** Convert bitset to string.
 * Only supports printing to binary.
 * \param bits: the bitset to stringify
 * \param sep: the memory separation formatting flag
 * \return string equivalent of bitset
 */
template<size_t LONGNESS>
std::string stringify_bitset(const std::bitset<LONGNESS>& bits,
							 IOFormatMemSep sep = IOFormatMemSep::all)
{
	// Avoiding magic numbers.
	const size_t BYTE_SIZE = 8;
	const size_t WORD_SIZE = 64;

	// Without memory separation formatting, string length is bitset length.
	size_t len = LONGNESS;

	// Add reserved space in string for byte/word separation, as requested.
	switch (sep) {
		case IOFormatMemSep::all:
			len += floor(LONGNESS / WORD_SIZE) * 2;
			[[fallthrough]];
		case IOFormatMemSep::byte:
			len += floor(LONGNESS / BYTE_SIZE);
			break;
		case IOFormatMemSep::word:
			len += floor(LONGNESS / WORD_SIZE);
			break;
		case IOFormatMemSep::none:
			break;
	}

	// Create a string with the necessary space reserved.
	std::string str = std::string();
	str.reserve(len);

	// Convert each bit to its binary string representation.
	for (size_t i = 0; i < LONGNESS; ++i) {
		str += (bits.test(i) ? "1" : "0");

		// Insert byte separation if desired.
		if (flags_check(sep, IOFormatMemSep::byte) &&
			(i + 1) % BYTE_SIZE == 0) {
			str += ' ';
		}

		// Insert word separation if desired.
		if (flags_check(sep, IOFormatMemSep::word) &&
			(i + 1) % WORD_SIZE == 0) {
			str += (flags_check(sep, IOFormatMemSep::byte)) ? "| " : "|";
		}
	}

	return str;
}

/** Convert integer representations of a single byte to a string.
 * Note: This uses separate (optimized) logic from stringify_integral!
 * \param byte: the byte to convert as an unsigned integer (uint8_t)
 * \param base: the base to use for the conversion (should be hex/bin/oct)
 * \param num_case: the case to use for digits > 9
 * \return a string representation of the byte
 */
inline std::string stringify_byte(const uint8_t byte,
						   IOFormatBase base = IOFormatBase::hex,
						   IOFormatNumCase num_case = IOFormatNumCase::upper)
{
	// Calculate the length of the string based on the desired base.
	size_t length = 0;

	switch (base) {
		case IOFormatBase::bin:
			length = 8;
			break;

		case IOFormatBase::oct:
			length = 4;
			break;

		case IOFormatBase::hex:
			length = 2;
			break;

		default:
			throw std::invalid_argument(
				"stringify_byte() only supports bases bin, oct, and hex");
	}

	// Reserve space in the string.
	std::string str = std::string();
	str.reserve(length);

	// Get a mutable copy of the integer.
	uint8_t val = byte;
	// Conver the base to an integer for use in the algorithm.
	unsigned int _base = static_cast<unsigned int>(base);

	/* Get digits for all positions in the number, thereby zero-padding.
	 * Store each digit directly in the appropriate position, so we don't
	 * have to reverse. */
	for (size_t i = 0; i < length; ++i) {
		size_t digit = val % _base;

		switch (num_case) {
			case IOFormatNumCase::lower:
				str += DIGIT_CHARS_LOWER[digit];
				break;

			case IOFormatNumCase::upper:
				str += DIGIT_CHARS_UPPER[digit];
				break;
		}

		val /= _base;
	}

	return str;
}

/** Convert integer representations of binary data to a string.
 * Note: This uses separate (optimized) logic from stringify_integral!
 * \param bytes: the integer representing the binary data
 * \param sep: which separators to use in the string representation
 * \param base: the base to use for the conversion (should probably be
 * hex/bin/oct) \param num_case: the case to use for digits > 9 \return the
 * string representation of the data
 */
template<typename T>
std::string stringify_bytes(const T& bytes,
							IOFormatMemSep sep = IOFormatMemSep::none,
							IOFormatBase base = IOFormatBase::hex,
							IOFormatNumCase num_case = IOFormatNumCase::upper)
{
	// Reserve is presently not practical on this string because of seperators.
	std::string str = std::string();

	// Parse each byte in the number, from last to first.
	for (int i = sizeof(T) - 1; i >= 0; --i) {
		// Convert the byte to string.
		str += stringify_byte(static_cast<uint8_t>(bytes >> (8 * i)),
							  base,
							  num_case);

		// Insert byte separation if desired.
		if (static_cast<bool>(sep & IOFormatMemSep::byte)) {
			str += ' ';
		}

		// Insert word separation if desired.
		if (static_cast<bool>(sep & IOFormatMemSep::word) && i + 1 % 8 == 0) {
			str += (static_cast<bool>(sep & IOFormatMemSep::byte)) ? "| " : "|";
		}
	}

	// We never use prefixes on memory dumps.

	return str;
}

/** Convert vector representation of binary data to a string.
 * \param bytes: the vector representing the binary data
 * \param sep: which separators to use in the string representation
 * \param base: the base to use for the conversion (should probably be
 * hex/bin/oct) \param num_case: the case to use for digits > 9 \return the
 * string representation of the data
 */
inline std::string stringify_bytes(const std::vector<uint8_t>& bytes,
							IOFormatMemSep sep = IOFormatMemSep::none,
							IOFormatBase base = IOFormatBase::hex,
							IOFormatNumCase num_case = IOFormatNumCase::upper)
{
	// Reserve is presently not practical on this string because of seperators.
	std::string str = std::string();

	// Parse each byte in the vector, from last to first.
	for (size_t i = bytes.size(); i > 0; --i) {
		// Convert the byte to string.
		str += stringify_byte(bytes[i - 1], base, num_case);

		// Insert byte separation if desired.
		if (static_cast<bool>(sep & IOFormatMemSep::byte)) {
			str += ' ';
		}

		// Insert word separation if desired.
		if (static_cast<bool>(sep & IOFormatMemSep::word) && i + 1 % 8 == 0) {
			str += (static_cast<bool>(sep & IOFormatMemSep::byte)) ? "| " : "|";
		}
	}

	// We never use prefixes on memory dumps.

	return str;
}

/** Convert binary data captured by MemLens to a string.
 * \param lens: the MemLens to acquire binary data from
 * \param sep: which separators to use in the string representation
 * \param base: the base to use for the conversion (should probably be
 * hex/bin/oct) \param num_case: the case to use for digits > 9 \return the
 * string representation of the data
 */
inline std::string stringify_bytes(const MemLens& lens,
							IOFormatMemSep sep = IOFormatMemSep::none,
							IOFormatBase base = IOFormatBase::hex,
							IOFormatNumCase num_case = IOFormatNumCase::upper)
{
	return stringify_bytes(lens.memory(), sep, base, num_case);
}

/**Convert a pointer (via MemLens) to a string representing the address.
 * Does not export the flag value, only the memory address.
 * \param the pointer integer to convert
 * \param letter case to use for digits greater than 9
 * \return the string representing the address */
inline std::string stringify_address(
	const MemLens& lens,
	const IOFormatNumCase& num_case = IOFormatNumCase::upper)
{
	/* Define the null address for a 64-bit system.
	 * We'll partially overwrite with the address to achieve zero padding.
	 * We add two extra zeroes to the front to facilitate the 0x prefix. */
	std::string str = std::string(16 + 2, '0');
	// Replace in the 'x' to create the 0x prefix.
	str.replace(1, 1, "x");

	// If the address is null, just return the null address string.
	if (lens.address() == 0) {
		return str;
	}

	/* Convert the address to a hexadecimal integer,
	 * but leave off the prefix, as that'd added in this function. */
	std::string addr = stringify_integral(lens.address(),
										  IOFormatBase::hex,
										  IOFormatSign::automatic,
										  num_case,
										  IOFormatBaseNotation::none);

	// Replace the integer string we received into the address string.
	str.replace(str.length() - addr.length(), addr.length(), addr);

	return str;
}

inline std::string stringify_pointer_data(const MemLens& lens)
{
	std::string str = "";
	std::string type = stringify_type(lens.data_type());

	switch (lens.pointer_type()) {
		case PtrType::raw:
			str = "[" + type + "*]";
			break;
		case PtrType::shared:
			str = "[shared_ptr<" + type + ">]";
			break;
		case PtrType::weak:
			str = "[weak_ptr<" + type + ">]";
			break;
	}

	return str;
}

#endif
