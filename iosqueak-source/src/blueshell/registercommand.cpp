#include "../include/iosqueak/blueshell.hpp"

// A function that registers tests that are stored in 'available_tests' map.
int Blueshell::register_command(std::string func_name,
								reg_command func,
								std::string description)
{
	// Creates a variable from the arguments to add
	stored_commands[func_name] = {description, std::bind(func, this, _1)};

	return 0;
}
