#include "../include/iosqueak/blueshell.hpp"

void Blueshell::registerdefaults()
{
	// Stores the commands in the stored_commands map.
	Blueshell::
		register_command("help",
						 std::bind(&Blueshell::help, this, _1),
						 "This menu",
						 "This menu displays what commands are available.",
						 0);
	Blueshell::register_command("clear",
								std::bind(&Blueshell::clear_screen, this, _1),
								"Clears the screen.",
								"Use this command to clear the screen.",
								0);
	Blueshell::
		register_command("history",
						 std::bind(&Blueshell::history, this, _1),
						 "Shows previously entered commands",
						 "This command will show previously used commands that "
						 "can be called up again using !# (eg. !2)",
						 0);
	Blueshell::
		register_command("list_commands",
						 std::bind(&Blueshell::list_commands, this, _1),
						 "Shows all available commands (not suites).",
						 "This command will show all the commands available (not suites). "
						 "If not registered, they will not be listed here.",
						 0);
}
