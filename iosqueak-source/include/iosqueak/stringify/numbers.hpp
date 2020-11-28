/** Stringify: Numbers [IOSqueak]
 *  Version 1.0
 *
 *  Convert integer and float types to strings.
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

#ifndef IOSQUEAK_STRINGIFY_NUMBERS_HPP
#define IOSQUEAK_STRINGIFY_NUMBERS_HPP

#include <cmath>
#include <string>
#include <type_traits>

#include "iosqueak/ioformat.hpp"
#include "iosqueak/stringify/utilities.hpp"
#include "iosqueak/tools/memlens.hpp"

const char* DIGIT_CHARS_UPPER = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* DIGIT_CHARS_LOWER = "0123456789abcdefghijklmnopqrstuvwxyz";

/**Count the number of characters necessary to represent an integer
 * as a string. Does not count the null terminator.
 * Based on http://stackoverflow.com/a/1489873/472647
 * \param the number to convert to a string
 * \param the base, default decimal
 * \return the calculated string length */
template<typename T>
size_t lengthify_integral(
	const T& val,
	const IOFormatBase& base = IOFormatBase::dec,
	const IOFormatSign& sign = IOFormatSign::automatic,
	const IOFormatBaseNotation notation = IOFormatBaseNotation::prefix)
{
	unsigned int _base = static_cast<unsigned int>(base);

	/* Count the negative sign if the number is negative, or if signs should
	 * always be included.
	 * If the number is literally "0", the length is also 1.
	 * For all positive numbers, start from 0. */
	size_t length = (val <= 0 || sign == IOFormatSign::always) ? 1 : 0;

	if (notation == IOFormatBaseNotation::prefix &&
		(_base == 2 || _base == 3 || _base == 8 || _base == 12 ||
		 _base == 16)) {
		length += 2;
	} else if (notation != IOFormatBaseNotation::none) {
		if (_base < 10) {
			length += 2;
		} else if (_base > 10) {
			length += 3;
		}
	}

	// Get the absolute value of the integer.
	T number = (val >= 0) ? val : -val;

	// Count the number of digits in the absolute value of the integer.
	while (number) {
		number /= _base;
		++length;
	}

	return length;
}

/**Count the number of characters necessary to represent a floating-point
 * number as a string. Does not count the null terminator.
 * WARNING: This is designed to strategically overcount, to prevent fatal
 * array size errors. This is necessary, since floats are unpredictable by
 * nature.
 * Based on http://stackoverflow.com/a/1489873/472647
 * \param the number to count the characters in
 * \param the number of decimal places (default 14)
 * \param whether to use scientific notation
 * \return the calculated string length */
template<typename T>
size_t lengthify_floating_point(
	const T& val,
	const IOFormatDecimalPlaces& places = IOFormatDecimalPlaces(14),
	const IOFormatSciNotation& sci = IOFormatSciNotation::automatic,
	const IOFormatSign& sign = IOFormatSign::automatic)
{
	// Get the absolute value of the number.
	T number = (val >= 0) ? val : -val;

	int magnitude = 0;
	bool useExp = false;

	switch (sci) {
		case IOFormatSciNotation::always:
			magnitude = log10(val);
			useExp = true;
			break;
		case IOFormatSciNotation::automatic:
			magnitude = log10(val);
			useExp = (magnitude > 12 || magnitude < -5);
			break;
		case IOFormatSciNotation::never:
		default:
			// Use current default value.
			break;
	}

	/* If the number is positive, start with 1 for the decimal point.
	 * If negative, also add 1 to the length for the negative sign. */
	size_t len = (val < 0 || sign == IOFormatSign::always) ? 2 : 1;

	if (useExp) {
		// Add space for the whole digit and the 'e'
		len += 2;
		// Show up to the specified number of decimal places.
		len += places.places;
		// Add space for the magnitude number and its sign.
		len += lengthify_integral(magnitude,
								  IOFormatBase::dec,
								  IOFormatSign::always);
	} else {
		// Add space for the whole part of the number.
		len += lengthify_integral(static_cast<int>(floor(number)));
		// Add space for the specified number of decimal places.
		len += places.places;
	}

	return len;
}

/* Convert an integer to a string
 * \param the integer to convert
 * \param the base for the conversion, default base-10
 * \param letter case to use for digits greater than 9
 * \param the notation to use for non-decimal bases.
 * \return the string representing the value.
 */
template<typename T>
std::string stringify_integral(
	const T& val,
	const IOFormatBase& base = IOFormatBase::dec,
	const IOFormatSign& sign = IOFormatSign::automatic,
	const IOFormatNumCase& num_case = IOFormatNumCase::upper,
	const IOFormatBaseNotation& notation = IOFormatBaseNotation::prefix)
{
	/* IMPLEMENTATION MEMO: itos() is not portable, and std::to_string()
	   does not support multiple bases at this time. Therefore, a custom
	   solution is called for. */

	// If the number is zero, skip everything else and return that.
	if (val == 0) {
		return "0";
	}

	unsigned int _base = static_cast<unsigned int>(base);

	/* Create a new string with a reserved length matching the expected
	 * length of the string. */
	std::string str = std::string();
	str.reserve(lengthify_integral(val, base, sign));

	bool prefix = false;

	if (notation == IOFormatBaseNotation::prefix &&
		(_base == 2 || _base == 3 || _base == 8 || _base == 12 ||
		 _base == 16)) {
		prefix = true;
	} else if (_base != 10 && notation != IOFormatBaseNotation::none) {
		str += DIGIT_CHARS_LOWER[_base % 10];
		if (_base > 10) {
			str += DIGIT_CHARS_LOWER[_base / 10];
		}
		str += '_';
	}

	/* Make a copy of the number for mutating.
	 * If it's negative, make it positive. */
	T number = (val >= 0) ? val : -val;

	while (number) {
		size_t digit = number % _base;
		switch (num_case) {
			case IOFormatNumCase::lower:
				str += DIGIT_CHARS_LOWER[digit];
				break;
			case IOFormatNumCase::upper:
				str += DIGIT_CHARS_UPPER[digit];
				break;
		}
		number /= _base;
	}

	if (prefix) {
		switch (_base) {
			case 2:
				str += 'b';
				break;
			case 3:
				str += 't';
				break;
			case 8:
				str += 'o';
				break;
			case 12:
				str += 'z';
				break;
			case 16:
				str += 'x';
				break;
		}
		str += '0';
	}

	if (val < 0) {
		str += "-";
	} else if (sign == IOFormatSign::always) {
		str += "+";
	}

	return reversify(str);
}

std::string stringify_char(
	const char& val, const IOFormatCharValue& as = IOFormatCharValue::as_char)
{
	switch (as) {
		case IOFormatCharValue::as_char:
			return std::string(1, val);
		case IOFormatCharValue::as_int:
			return stringify_integral(val, IOFormatBase::dec);
	}
	return "";
}

/**Convert a floating point number to a string.
 * \param the number to convert to a string
 * \param the number of decimal places (default 14)
 * \param whether to use scientific notation
 * \return the string representing the value. */
template<typename T>
std::string stringify_floating_point(
	const T& val,
	const IOFormatDecimalPlaces& places = IOFormatDecimalPlaces(14),
	const IOFormatSciNotation& sci = IOFormatSciNotation::automatic,
	const IOFormatSign& sign = IOFormatSign::automatic)
{
	/* Create a new string with a reserved length matching the expected
	 * length of the string. */
	std::string str = std::string();
	str.reserve(lengthify_integral(val, IOFormatBase::dec, sign));

	// Get the absolute value of the number.
	T number = (val >= 0) ? val : -val;

	int magnitude = 0;
	bool useExp = false;

	switch (sci) {
		case IOFormatSciNotation::always:
			magnitude = log10(number);
			useExp = true;
			break;
		case IOFormatSciNotation::automatic:
			magnitude = log10(number);
			useExp = (magnitude > 12 || magnitude < -5);
			break;
		case IOFormatSciNotation::never:
		default:
			// Use current default value.
			break;
	}

	// Append appropriate sign
	if (val < 0) {
		str += '-';
	} else if (sign == IOFormatSign::always) {
		str += '+';
	}

	if (useExp) {
		// Modify to D.NNNNNNNN form and convert that to a string.
		T modified = number * pow(10, -(ceil(log10(number))) + 1);
		str += stringify_floating_point(modified,
										places,
										IOFormatSciNotation::never);

		// Add scientific notation at end
		str += 'e';
		std::string str_mag = stringify_integral(magnitude,
												 IOFormatBase::dec,
												 IOFormatSign::always);
		str += str_mag;
	} else {
		// Get whole part
		long long int whole = static_cast<int>(floor(number));
		str += stringify_integral(whole);

		// Append decimal character
		str += '.';

		// Get decimal part
		long long int decimal =
			floor((number - whole) * pow(10, places.places));
		str += stringify_integral(decimal);
	}

	return str;
}

#endif
