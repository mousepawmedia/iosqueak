#include "../include/iosqueak/blueshell.hpp"

size_t Blueshell::tab_press(std::string& sent_command)
{
	int keypress{Blueshell::getch()};

	if (keypress != 9) {
		return keypress;
	}
	/* If tab is pressed again, and command is
	 *  empty, display all available commands */
	if (sent_command.empty()) {
		channel << IOCtrl::n;
		for (auto& cmd : stored_commands) {
			channel << cmd.first << '\t';
		}
		channel << IOCtrl::endl;
	}

	/* If tab is pressed again, and command is not
	 *  empty, find any commands that start with
	 *  command argument. */
	if (!sent_command.empty()) {
		std::deque<std::string> temp_cmds;
		for (auto& cmd : stored_commands) {
			/* Looks for any commands that match the
			 *  'command' sent. */
			if (cmd.first.rfind(sent_command, 0) == 0) {
				temp_cmds.push_back(cmd.first);
			}
		}

		if (!temp_cmds.empty()) {
			/* If there is only one item in the deque
			 *  then print the command to the screen.
			 *  Otherwise print all items in deque.*/
			if (temp_cmds.size() == 1) {
				sent_command = temp_cmds.front();
				channel << temp_cmds.front() << IOCtrl::end;
			} else {
				channel << IOCtrl::n;
				for (auto& cmd : temp_cmds) {
					channel << cmd << '\t';
				}
				channel << IOCtrl::endl;

				/* Fill in any chars that match each string in deque.
				 *  For example 'te' in words 'test' and 'tests' would
				 *  fill in the partial typed command 'te' to 'test'.
				 *  Get the size of the largest word, and use that for
				 *  the end of the loop. */
				for (size_t i{sent_command.size()};
					 i < std::size(*max_element(
							 temp_cmds.begin(),
							 temp_cmds.end(),
							 [](auto& word1, auto& word2) {
								 return word1.size() < word2.size();
							 }));
					 ++i) {
					if (std::all_of(temp_cmds.begin(),
									temp_cmds.end(),
									[&](auto& word) {
										return word[i] == temp_cmds[0][i];
									})) {
						sent_command.push_back(temp_cmds[0][i]);
						continue;
					}
					break;
				}
			}
		}
		temp_cmds.clear();
	}
	return 0;
}
