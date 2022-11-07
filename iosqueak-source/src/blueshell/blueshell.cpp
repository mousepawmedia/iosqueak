#include "../include/iosqueak/blueshell.hpp"

Blueshell::Blueshell(std::string sent_name) : shell_name(sent_name)
{
	// ctor
}

Blueshell::~Blueshell()
{
	// dtor
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

// Lists all of the registered commands.
int Blueshell::list_commands(arguments&)
{
	channel << IOFormatTextFG::green << IOCtrl::n
			<< "These are the commands currently available:"
			<< IOFormatTextFG::white << IOCtrl::n;
	for (auto& cmd : stored_commands.get_set()) {
		channel << cmd.func_name << '\t';
	}
	channel << IOCtrl::endl;

	return 0;
}
