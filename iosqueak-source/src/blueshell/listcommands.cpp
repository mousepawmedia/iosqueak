#include "../include/iosqueak/blueshell.hpp"

// Lists all of the registered commands.
int Blueshell::list_commands(arguments&)
{
    size_t count{0};
	channel << IOFormatTextFG::green << IOCtrl::n
			<< "These are the commands currently available:"
			<< IOFormatTextFG::white << IOCtrl::n << "exit"
			<< IOFormatTextFG::green << " or " << IOFormatTextFG::white
			<< "quit" << IOFormatTextFG::green << " to end session."
			<< IOFormatTextFG::white << IOCtrl::n;
		for (auto& cmd : stored_commands.get_set()) {
			channel << cmd.func_name;
			if (count < 2) {
				++count;
				channel << '\t';
			} else {
				channel << IOCtrl::n;
				count = 0;
			}
		}
	channel << IOCtrl::endl;

	return 0;
}
