#include "../include/iosqueak/blueshell.hpp"

int Blueshell::history(arguments& searched_word)
{
	channel << IOCtrl::n << IOCtrl::end;
	/* If there is no word specified, print out
	 * entire history. */
	if (searched_word.empty()) {
		std::for_each(previous_commands.rbegin(),
					  previous_commands.rend(),
					  [](const auto& cmd) {
						  channel << cmd.first << "  " << cmd.second
								  << IOCtrl::n << IOCtrl::end;
					  });
	} else {
		// Loop through searched_word for searched items.
		for (auto& word : searched_word) {
			// Search for any matching command in previous_commands container.
			std::for_each(previous_commands.rbegin(),
						  previous_commands.rend(),
						  [this, &word](auto& searched) {
							  if (word ==
								  Blueshell::tokens(searched.second).front()) {
								  channel << searched.first << ' '
										  << searched.second << IOCtrl::n;
							  }
							  channel << IOCtrl::end;
						  });
		}
	}
	channel << "Use ! to recall a command (eg. !2)" << IOCtrl::endl;

	return 0;
}
