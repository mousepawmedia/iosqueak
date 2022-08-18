#include "../include/iosqueak/blueshell.hpp"

Blueshell::Blueshell() {}

Blueshell::~Blueshell()
{
	// dtor
}

// A function that just clears the screen.
int Blueshell::clear_screen(std::string)
{
	channel << "\033[2J\033[1;1H" << IOCtrl::end;
	return 0;
}

// Prints command to screen, by erasing the line first.
void Blueshell::print_line(const std::string& command)
{
	channel << IOCtrl::r << "\x1b[2K"
			<< ">>> " << command << IOCtrl::end;
}

// Delete when done with Blueshell code
int Blueshell::test(std::string)
{
	std::cout << "\nRunning test function.\n";
	return 0;
}

void Blueshell::backspace(std::string& command)
{
	std::cout << "\x1b[8";
	if (command.size() != 0) {
		command.pop_back();
	}
}
