#include "../include/iosqueak/blueshell.hpp"

void Blueshell::process_command(std::string& sent_command)
{
	std::stringstream ss(sent_command);
	std::string first_command;

	// Take first word from sent command to check if valid command.
	ss >> first_command;

	// Place the sent_command in a string for processing.
	std::string options{ss.str()};

	// Check if the command is available, if not send "Unknown command".
	auto it{stored_commands.find(first_command)};
	if (it != stored_commands.end()) {
		(it->second.second)(first_command);
		/* Adds command to previous_commands container. If it is not
		 *  the last command added to the deque. */
		if (previous_commands.front().second != sent_command) {
			previous_commands.push_front(
				std::pair((previous_commands.size() == 0)
							  ? 1
							  : previous_commands.front().first + 1,
						  sent_command));
		}
	} else {
		channel << IOCtrl::n << sent_command << " Unknown command"
				<< IOCtrl::endl;
	}

	// Break rest of sent_command into strings to process options/flags
	Blueshell::process_options(options);
}
