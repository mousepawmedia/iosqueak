#include "../include/iosqueak/blueshell.hpp"

int Blueshell::history(std::string)
{
	channel << IOCtrl::n << IOCtrl::end;

	std::for_each(previous_commands.rbegin(),
				  previous_commands.rend(),
				  [](const auto& cmd) {
					  channel << cmd.first << "  " << cmd.second << IOCtrl::n
							  << IOCtrl::end;
				  });
	channel << "Use ! to recall a command (eg. !2)" << IOCtrl::endl;
	return 0;
}
