#include "../include/iosqueak/blueshell.hpp"

// A function to show what commands are available.
int Blueshell::help(std::string)
{
	channel << IOFormatTextFG::green << IOCtrl::n;
	channel << "****************************************" << IOCtrl::n;
	channel << "*                 Help                 *" << IOCtrl::n;
	channel << "* The following commands are available *" << IOCtrl::n;
	channel << "****************************************" << IOCtrl::n;
	channel << "\nquit or exit to leave Blueshell" << IOCtrl::endl;

	for (auto& cmd : stored_commands) {
		//		channel << '\t'<< cmd.first << IOCtrl::n;
		//		channel << cmd.second.first<< IOCtrl::n<<IOCtrl::n;
		channel << cmd.first << '\t' << cmd.second.first << IOCtrl::n;
	}
	channel << IOCtrl::endl;

	return 0;
}
