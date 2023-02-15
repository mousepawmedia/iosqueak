#include "../include/iosqueak/blueshell.hpp"

Blueshell::Blueshell(std::string sent_name) : shell_name(sent_name)
{
	// ctor
}

// A function that just clears the screen.
int Blueshell::clear_screen(arguments&)
{
	channel << "\033[2J\033[1;1H" << IOCtrl::end;
	return 0;
}

// Prints command to screen, by erasing the line first.
void Blueshell::print_line(const std::string& sent_command)
{
	// Create a string to place cursor position.
	std::string place_cursor{
		"\x1b[" + std::to_string((sent_command.size() + 4) - cursor_moves) +
		"C"};

	/* Carriage return followed by clearing the line.
	 * Then print >>> and the command. Set cursor position.*/
	channel << IOCtrl::r << "\x1b[2K"
			<< ">>> " << sent_command << IOCtrl::r << place_cursor
			<< IOCtrl::end;
}
