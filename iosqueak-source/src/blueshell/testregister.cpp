#include "../include/iosqueak/blueshell/testregister.hpp"

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

int TestRegister::test_attempt(const Alias::arguments& options)
{
	std::cout << "\nRunning testregister test_attempt.\n";
	for (const auto& cmd : options) {
		std::cout << cmd << '\n';
	}
	std::cout << std::endl;
	
	return 0;
}
