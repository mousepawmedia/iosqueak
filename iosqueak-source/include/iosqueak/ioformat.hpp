/** IOFormat [IOSqueak]
 * Version: 2.0.0
 *
 * Formatting flags for stringify and channel output.
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

#ifndef IOSQUEAK_IOFORMAT_HPP
#define IOSQUEAK_IOFORMAT_HPP

#include <string>

#include "iosqueak/utilities/bitfield.hpp"

/// Defines integer base (radix)
enum class IOFormatBase {
	bin = 2,
	b2 = 2,
	ter = 3,
	b3 = 3,
	quat = 4,
	b4 = 4,
	quin = 5,
	b5 = 5,
	sen = 6,
	b6 = 6,
	sep = 7,
	b7 = 7,
	oct = 8,
	b8 = 8,
	b9 = 9,
	dec = 10,
	b10 = 10,
	und = 11,
	b11 = 11,
	duo = 12,
	doz = 12,
	b12 = 12,
	tri = 13,
	b13 = 13,
	tetra = 14,
	b14 = 14,
	pent = 15,
	b15 = 15,
	hex = 16,
	b16 = 16,
	b17 = 17,
	b18 = 18,
	b19 = 19,
	vig = 20,
	b20 = 20,
	b21 = 21,
	b22 = 22,
	b23 = 23,
	b24 = 24,
	b25 = 25,
	b26 = 26,
	b27 = 27,
	b28 = 28,
	b29 = 29,
	b30 = 30,
	b31 = 31,
	b32 = 32,
	b33 = 33,
	b34 = 34,
	b35 = 35,
	b36 = 36
};

/// Indicates how base (radix) should be specified on an integer.
enum class IOFormatBaseNotation {
	/* Use prefix whenever possible: 0b (2), 0t (3), 0o (8), 0x (16), 0z (12)
	 * and subscript for all other bases. */
	prefix,
	// Subscript only, even if there's an available prefix
	subscript,
	// No base notation at all
	none
};

/// Defines how boolean values should be represented textually.
enum class IOFormalBoolStyle {
	lower = 0,
	upper = 1,
	caps = 2,
	numeral = 3,
	test = 4,
	scott = 5
};

/// Defines whether char should be processes as a literal char or an integer.
enum class IOFormatCharValue { as_char = 0, as_int = 1 };

/// Defines how many decimal places should be shown in a floating-point number.
struct IOFormatDecimalPlaces {
	explicit IOFormatDecimalPlaces(unsigned int p) : places(p) {}

	int places = 14;
};

/// Defines how raw memory is formatted, in terms of spacing.
enum class IOFormatMemSep {
	/// Output as one long string.
	none = 0,
	/// Output with spaces between bytes.
	byte = (1 << 0),
	/// Output with bars between words (8 bytes).
	word = (1 << 1),
	/// Output with spaces between bytes and bars between words.
	all = (1 << 0) | (1 << 1)
};

// IOFormatMemSep is a bitfield, so it needs bitwise support.
MAKE_BITFIELD(IOFormatMemSep);

/// Define the desired case for digits above 9.
enum class IOFormatNumCase {
	/// Print all letter digits as lowercase.
	lower = 0,
	/// Print all letter digits as uppercase.
	upper = 1
};

/// Define how a pointer should be interpreted.
enum class IOFormatPtr {
	/// Print the value at the address.
	value = 0,
	/// Print the actual memory address.
	address = 1,
	/// Display the pointer metadata.
	pointer = 2,
	/// Dump the hexadecimal representation of the memory at address.
	memory = 3,
};

/// Define whaether scientific notation should be used.
enum class IOFormatSciNotation {
	/// Turn off all scientific notation.
	never = 0,
	/// Automatically select the best option.
	automatic = 1,
	/// Turn on all scientific notation.
	always = 2
};

/// Defines whether to show the positive/negative sign on a number.
enum class IOFormatSign {
	/// Show only the negative sign (default).
	automatic = 0,
	/// Show positive or negative sign.
	always = 1
};

/// Defines the standard used for displaying text attributes and colors.
enum class IOFormatStandard { none = 0, ansi = 1 };

/** Text attributes, based on ANSI control codes.
 * Not all attributes will be recognized by all terminals.
 */
enum class IOFormatTextAttr {
	/// Turn off all attributes.
	none = 0,
	bold = (1 << 0),
	faint = (1 << 1),
	italic = (1 << 2),
	underline = (1 << 3),
	blink_slow = (1 << 4),
	blink_fast = (1 << 5),
	// Invert is also Reverse Video
	invert = (1 << 6),
	invisible = (1 << 7),
	strikethrough = (1 << 8),
	double_underline = (1 << 9),
	// Same option turns off bold and faint
	no_bold = (1 << 10),
	no_faint = (1 << 10),
	no_italic = (1 << 11),
	// Same option turns off both single and double underline
	no_underline = (1 << 12),
	no_slow_blink = (1 << 13),
	no_fast_blink = (1 << 14),
	no_invert = (1 << 15),
	no_invisible = (1 << 16),
	no_strikethorugh = (1 << 17)
};

// Interpret IOFormatTextAttr as a bitfield, for applying multiple attributes.
MAKE_BITFIELD(IOFormatTextAttr);

/// Define text background color, based on ANSI control codes.
enum class IOFormatTextBG {
	none = 0,
	black = 1,
	red = 2,
	green = 3,
	yellow = 4,
	blue = 5,
	magenta = 6,
	cyan = 7,
	white = 8
};

/// Define text background color, based on ANSI control codes.
enum class IOFormatTextFG {
	none = 0,
	black = 1,
	red = 2,
	green = 3,
	yellow = 4,
	blue = 5,
	magenta = 6,
	cyan = 7,
	white = 8
};

/// Stores a complete set of attributes and formatting flags.
class IOFormat
{
private:
	IOFormatBase fmt_base;
	IOFormatBaseNotation fmt_base_notation;
	IOFormalBoolStyle fmt_bool_style;
	IOFormatCharValue fmt_char_value;
	IOFormatDecimalPlaces fmt_decimal_places;
	IOFormatMemSep fmt_mem_sep;
	IOFormatNumCase fmt_numeral_case;
	IOFormatPtr fmt_ptr;
	IOFormatSciNotation fmt_sci_notation;
	IOFormatSign fmt_sign;
	IOFormatStandard fmt_standard;
	IOFormatTextAttr fmt_text_attr;
	IOFormatTextBG fmt_text_bg;
	IOFormatTextFG fmt_text_fg;

public:
	/// Default constructor
	IOFormat()
	: fmt_base(IOFormatBase::b10),
	  fmt_base_notation(IOFormatBaseNotation::prefix),
	  fmt_bool_style(IOFormalBoolStyle::lower),
	  fmt_char_value(IOFormatCharValue::as_char),
	  fmt_decimal_places(IOFormatDecimalPlaces(14)),
	  fmt_mem_sep(IOFormatMemSep::all),
	  fmt_numeral_case(IOFormatNumCase::upper), fmt_ptr(IOFormatPtr::value),
	  fmt_sci_notation(IOFormatSciNotation::automatic),
	  fmt_sign(IOFormatSign::automatic), fmt_standard(IOFormatStandard::ansi),
	  fmt_text_attr(IOFormatTextAttr::none), fmt_text_bg(IOFormatTextBG::none),
	  fmt_text_fg(IOFormatTextFG::none)
	{
	}

	/// Copy constructor
	IOFormat(const IOFormat& cpy)
	: fmt_base(cpy.fmt_base), fmt_base_notation(cpy.fmt_base_notation),
	  fmt_bool_style(cpy.fmt_bool_style), fmt_char_value(cpy.fmt_char_value),
	  fmt_decimal_places(cpy.fmt_decimal_places), fmt_mem_sep(cpy.fmt_mem_sep),
	  fmt_numeral_case(cpy.fmt_numeral_case), fmt_ptr(cpy.fmt_ptr),
	  fmt_sci_notation(cpy.fmt_sci_notation), fmt_sign(cpy.fmt_sign),
	  fmt_standard(cpy.fmt_standard), fmt_text_attr(cpy.fmt_text_attr),
	  fmt_text_bg(cpy.fmt_text_bg), fmt_text_fg(cpy.fmt_text_fg)
	{
	}

	/// \return the current IOFormatBase flag
	const IOFormatBase& base() const { return fmt_base; }

	/// \return the current IOFormatBaseNotation flag
	const IOFormatBaseNotation& base_notation() const
	{
		return fmt_base_notation;
	}

	/// \return the current IOFormalBoolStyle flag
	const IOFormalBoolStyle& bool_style() const { return fmt_bool_style; }

	/// \return the current IOFormatCharValue flag
	const IOFormatCharValue& char_value() const { return fmt_char_value; }

	/// \return the current IOFormatDecimalPlaces flag
	const IOFormatDecimalPlaces& decimal_places() const
	{
		return fmt_decimal_places;
	}

	/// \return the current IOFormatMemSep flag
	const IOFormatMemSep& mem_sep() const { return fmt_mem_sep; }

	/// \return the current IOFormatNumCase flag
	const IOFormatNumCase& numeral_case() const { return fmt_numeral_case; }

	/// \return the current IOFormatPtr flag
	const IOFormatPtr& ptr() const { return fmt_ptr; }

	/// \return the current IOFormatSciNotation flag
	const IOFormatSciNotation& sci_notation() const { return fmt_sci_notation; }

	/// \return the current IOFormatSign flag
	const IOFormatSign& sign() const { return fmt_sign; }

	/// \return the current IOFormatStandard flag
	const IOFormatStandard& standard() const { return fmt_standard; }

	/// \return the current IOFormatTextAttr flag
	const IOFormatTextAttr& text_attr() const { return fmt_text_attr; }

	/** Returns the control codes for the text attributes.
	 * \param standard: the attribute standard to use
	 * \return a std::string specifying the text attributes.
	 */
	const std::string text_attr(const IOFormatStandard& standard) const;

	/// \return the current IOFormatTextBG flag
	const IOFormatTextBG& text_bg() const { return fmt_text_bg; }

	/** Returns the control codes for the text background color.
	 * \param standard: the attribute standard to use
	 * \return a std::string specifying the text background color.
	 */
	const std::string text_bg(const IOFormatStandard& standard) const;

	/// \return the current IOFormatTextFG flag
	const IOFormatTextFG& text_fg() const { return fmt_text_fg; }

	/** Returns the control codes for the text foreground color.
	 * \param standard: the attribute standard to use
	 * \return a std::string specifying the text foreground color.
	 */
	const std::string text_fg(const IOFormatStandard& standard) const;

	/** Generate a terminal format string (e.g. ANSI SGR) for the current
	 * formatting flags, according to the specified standard.
	 * \param standard: the attribute standard to use
	 * \return the terminal format string.
	 */
	const std::string format_string(const IOFormatStandard& standard) const;

	/** Generate a terminal format string (e.g. ANSI SGR) for the current
	 * formatting flags, according to the standard specified in the IOFormat
	 * object.
	 * \return the terminal format string.
	 */
	const std::string format_string() const
	{
		return format_string(this->fmt_standard);
	}

	/// Reset the text attributes and colors, but nothing else.
	void reset_attributes();

	/// Copy assignment operator.
	IOFormat& operator=(const IOFormat& cpy);

	// Inject formatting flags into IOFormat object...

	IOFormat& operator<<(const IOFormatBase& rhs)
	{
		fmt_base = rhs;
		return *this;
	}
	IOFormat& operator<<(const IOFormatBaseNotation& rhs)
	{
		fmt_base_notation = rhs;
		return *this;
	}
	IOFormat& operator<<(const IOFormalBoolStyle& rhs)
	{
		fmt_bool_style = rhs;
		return *this;
	}
	IOFormat& operator<<(const IOFormatCharValue& rhs)
	{
		fmt_char_value = rhs;
		return *this;
	}
	IOFormat& operator<<(const IOFormatDecimalPlaces& rhs)
	{
		fmt_decimal_places = rhs;
		return *this;
	}
	IOFormat& operator<<(const IOFormatMemSep& rhs)
	{
		fmt_mem_sep = rhs;
		return *this;
	}
	IOFormat& operator<<(const IOFormatNumCase& rhs)
	{
		fmt_numeral_case = rhs;
		return *this;
	}
	IOFormat& operator<<(const IOFormatPtr& rhs)
	{
		fmt_ptr = rhs;
		return *this;
	}
	IOFormat& operator<<(const IOFormatSciNotation& rhs)
	{
		fmt_sci_notation = rhs;
		return *this;
	}
	IOFormat& operator<<(const IOFormatSign& rhs)
	{
		fmt_sign = rhs;
		return *this;
	}
	IOFormat& operator<<(const IOFormatStandard& rhs)
	{
		fmt_standard = rhs;
		return *this;
	}
	IOFormat& operator<<(const IOFormatTextAttr& rhs)
	{
		fmt_text_attr = rhs;
		return *this;
	}
	IOFormat& operator<<(const IOFormatTextBG& rhs)
	{
		fmt_text_bg = rhs;
		return *this;
	}
	IOFormat& operator<<(const IOFormatTextFG& rhs)
	{
		fmt_text_fg = rhs;
		return *this;
	}
};

#endif
