#include <iomanip>

#include "../include/iosqueak/blueshell.hpp"
// A function to show what commands are available.
int Blueshell::help(std::string)
{
	channel << IOFormatTextFG::green << IOCtrl::n;
	channel << "****************************************" << IOCtrl::n;
	channel << "*                 Help                 *" << IOCtrl::n;
	channel << "* The following commands are available *" << IOCtrl::n;
	channel << "****************************************" << IOCtrl::n;

	channel << IOFormatTextFG::white << "\nquit " << IOFormatTextFG::green
			<< "or " << IOFormatTextFG::white << "exit "
			<< IOFormatTextFG::green << "to leave " << shell_name << " shell"
			<< IOCtrl::n << IOCtrl::endl;
	for (auto& cmd : stored_commands) {
		// Set the command to white, and the description to green.
		std::cout << "\033[37m" << std::setw(20) << std::left << cmd.first
				  << "\033[38;2;0;255;0m" << cmd.second.first << '\n';
	}
	std::cout << "\033[37m";

	return 0;
}
