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

// std::string IODecorAlign
// {
// 	switch(IODecorAlign){
// 	case IODecorAlign::Center:
// 		IODecorCenter;
// 		break;
// 	case IODecorAlign::Right:
// 		 IODecorRight;
// 		break;
// 	case IODecorAlign::Left:
// 		 IODecorLeft;
// 		break;
// 	default:
// 		 IODecorLeft;
// };
	
	
// }

// std::string IODecorPad::to_string() const
// {


	
// }
