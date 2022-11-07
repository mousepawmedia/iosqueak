#include "../include/iosqueak/blueshell.hpp"

void Blueshell::check_quote(std::string& check_command)
{
	if (Blueshell::inner_quote) {
		check_command.push_back('\\');
		check_command.push_back('"');
		Blueshell::inner_quote = false;
	}
	if (Blueshell::inner_single) {
		check_command.push_back('\\');
		check_command.push_back('\'');
		Blueshell::inner_single = false;
	}
	if (Blueshell::outer_quote) {
		check_command.push_back('"');
		Blueshell::outer_quote = false;
	}
	if (Blueshell::outer_single) {
		check_command.push_back('\'');
		Blueshell::outer_single = false;
	}
}
