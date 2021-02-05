#include "iosqueak/ioformat.hpp"

const std::string IOFormat::text_attr(const IOFormatStandard& standard) const
{
	std::string str_text_attr = "";
	// https://mudhalla.net/tintin/info/ansicolor/
	if (standard == IOFormatStandard::ansi) {
		if (flags_check(fmt_text_attr, IOFormatTextAttr::none)) {
			str_text_attr += ";0";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::bold)) {
			str_text_attr += ";1";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::faint)) {
			str_text_attr += ";2";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::italic)) {
			str_text_attr += ";3";
		}

		if (flags_check(fmt_text_attr, IOFormatTextAttr::underline)) {
			str_text_attr += ";4";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::blink_slow)) {
			str_text_attr += ";5";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::blink_fast)) {
			str_text_attr += ";6";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::invert)) {
			str_text_attr += ";7";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::invisible)) {
			str_text_attr += ";8";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::double_underline)) {
			str_text_attr += ";9";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::strikethrough)) {
			str_text_attr += ";21";
		}

		// After turning ON features, turn OFF other features.
		if (flags_check(fmt_text_attr, IOFormatTextAttr::no_bold)) {
			str_text_attr += ";22";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::no_italic)) {
			str_text_attr += ";23";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::no_underline)) {
			str_text_attr += ";24";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::no_slow_blink)) {
			str_text_attr += ";25";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::no_fast_blink)) {
			str_text_attr += ";26";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::no_invert)) {
			str_text_attr += ";27";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::no_invisible)) {
			str_text_attr += ";28";
		}
		if (flags_check(fmt_text_attr, IOFormatTextAttr::no_strikethorugh)) {
			str_text_attr += ";28";
		}
	}
	return str_text_attr;
}

const std::string IOFormat::text_bg(const IOFormatStandard& standard) const
{
	if (standard == IOFormatStandard::ansi) {
		switch (fmt_text_bg) {
			case IOFormatTextBG::none:
				return ";49";
			case IOFormatTextBG::black:
				return ";40";
			case IOFormatTextBG::red:
				return ";41";
			case IOFormatTextBG::green:
				return ";42";
			case IOFormatTextBG::yellow:
				return ";43";
			case IOFormatTextBG::blue:
				return ";44";
			case IOFormatTextBG::magenta:
				return ";45";
			case IOFormatTextBG::cyan:
				return ";46";
			case IOFormatTextBG::white:
				return ";47";
		}
	}
	return "";
}

const std::string IOFormat::text_fg(const IOFormatStandard& standard) const
{
	if (standard == IOFormatStandard::ansi) {
		switch (fmt_text_fg) {
			case IOFormatTextFG::none:
				return ";39";
			case IOFormatTextFG::black:
				return ";30";
			case IOFormatTextFG::red:
				return ";31";
			case IOFormatTextFG::green:
				return ";32";
			case IOFormatTextFG::yellow:
				return ";33";
			case IOFormatTextFG::blue:
				return ";34";
			case IOFormatTextFG::magenta:
				return ";35";
			case IOFormatTextFG::cyan:
				return ";36";
			case IOFormatTextFG::white:
				return ";37";
		}
	}
	return "";
}

const std::string IOFormat::format_string(
	const IOFormatStandard& standard) const
{
	std::string format = "";
	switch (standard) {
		case IOFormatStandard::none:
			break;
		case IOFormatStandard::ansi:
			format += "\33[";
			format += text_attr(standard);
			format += text_bg(standard);
			format += text_fg(standard);
			format += "m";
	}
	return format;
}

void IOFormat::reset_attributes()
{
	this->fmt_text_attr = IOFormatTextAttr::none;
	this->fmt_text_fg = IOFormatTextFG::none;
	this->fmt_text_bg = IOFormatTextBG::none;
}

IOFormat& IOFormat::operator=(const IOFormat& cpy)
{
	fmt_base = cpy.fmt_base;
	fmt_base_notation = cpy.fmt_base_notation;
	fmt_bool_style = cpy.fmt_bool_style;
	fmt_char_value = cpy.fmt_char_value;
	fmt_decimal_places = cpy.fmt_decimal_places;
	fmt_mem_sep = cpy.fmt_mem_sep;
	fmt_numeral_case = cpy.fmt_numeral_case;
	fmt_ptr = cpy.fmt_ptr;
	fmt_sci_notation = cpy.fmt_sci_notation;
	fmt_sign = cpy.fmt_sign;
	fmt_standard = cpy.fmt_standard;
	fmt_text_attr = cpy.fmt_text_attr;
	fmt_text_bg = cpy.fmt_text_bg;
	fmt_text_fg = cpy.fmt_text_fg;
	return *this;
}
