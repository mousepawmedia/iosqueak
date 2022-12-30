#include "../include/iosqueak/testregister.hpp"

TestRegister::TestRegister()
{
	// Send Blueshell the name of your shell.
	Blueshell::shell_name = name;

	Blueshell::register_command(command_name,
								command,
								short_desc,
								long_desc,
								number_of_options,
								number_of_args);

	TestRegister::initial_shell();
}

TestRegister::~TestRegister()
{
	// dtor
}
