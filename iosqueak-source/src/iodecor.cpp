#include "iosqueak/iodecor.hpp"

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
