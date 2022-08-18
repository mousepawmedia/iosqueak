#include "../include/iosqueak/blueshell.hpp"

void Blueshell::bang(std::string& command)
{
	while (true) {
		Blueshell::print_line(command);
		int keypress{Blueshell::getch()};

		if (keypress == 10) {
			// Remove the '!' from the front of command.
			command.erase(std::find(command.begin(), command.end(), '!'));

			// Search for command matching the number entered.
			auto cmd =
				std::find_if(previous_commands.begin(),
							 previous_commands.end(),
							 [&command](const auto& cmd) {
								 return std::to_string(cmd.first) == command;
							 });

			// If found, send command, otherwise allow to enter another.
			if (cmd != previous_commands.end()) {
				command = cmd->second;
				return;
			} else {
				channel << "\nNo command matching " << command << IOCtrl::endl;
				command = "!";
				continue;
			}

			return;
		}
		if (keypress > 47 && keypress < 58) {
			command.push_back(keypress);
			Blueshell::print_line(command);
			continue;
		}
	}
}
