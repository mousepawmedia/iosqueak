#include "iosqueak/iodecor.hpp"
#include <iomanip>

unsigned short IODecor_Base::columns() { return Window::get_width(); }

std::ostream& operator<<(std::ostream& out, const IODecor_Base& d)
{
	out << d.to_string();
	return out;
}

std::string IODecorFill::to_string() const
{
	return std::string(columns(), fill_char);
}

std::string IODecorLeft::to_string() const
{
	unsigned short col = columns();
	size_t len = this->str.length();
	if (len > col) {
		return this->str;
	}
	unsigned short pad_len = (col - len - 1);
	std::string pad_right = std::string(pad_len, fill_char);
	return (this->str + " " + pad_right);
}

std::string IODecorCenter::to_string() const
{
	unsigned short col = columns();
	size_t len = this->str.length();
	if (len > col) {
		return this->str;
	}
	unsigned short pad_len = (col - len - 2);
	std::string pad_left = std::string(pad_len / 2, fill_char);
	std::string pad_right = std::string(pad_len / 2 + pad_len % 2, fill_char);
	return (pad_left + " " + this->str + " " + pad_right);
}

std::string IODecorRight::to_string() const
{
	unsigned short col = columns();
	size_t len = this->str.length();
	if (len > col) {
		return this->str;
	}
	unsigned short pad_len = (col - len - 1);
	std::string pad_left = std::string(pad_len, fill_char);
	return (pad_left + " " + this->str);
}


IODecorPad::IODecorPad(int width, IODecorAlign align) : width_(width), align_(align) {}

std::string IODecorPad::to_string() const {
    std::string alignment;
    switch (align_) {
        case IODecorAlign::left: alignment = "left"; break;
        case IODecorAlign::center: alignment = "center"; break;
        case IODecorAlign::right: alignment = "right"; break;
        default: alignment = "left"; break;
    }
    return "Width: " + std::to_string(width_) + ", Alignment: " + alignment;
}

std::ostream& operator<<(std::ostream& os, const IODecorPad& decor) {
	std::ios_base::fmtflags oldFlags = os.flags();
	switch (decor.align_)
	{
	case IODecorAlign::right:
		os << std::right << std::setw(decor.width_);
		break;
	case IODecorAlign::center:
		break;
	case IODecorAlign::left:
	default:
		os << std::left << std::setw(decor.width_);
		break;
	}

	os.flags(oldFlags);
	return os;
}