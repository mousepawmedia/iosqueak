#include "../include/iosqueak/blueshell.hpp"
#include "string.h"
void Blueshell::arrow_press(std::string& command)
{
	// This is for previous_commands vector call.
	size_t vec_size{0};
	std::string prev_cmd_holder;

	// Set check_return to 0, to make sure no previous setting.
	check_return = 0;

	while (true) {
		int keypress{Blueshell::getch()};

		// If tab is pressed, run tab_press.
		if (keypress == 9) {
			command = prev_cmd_holder;
			Blueshell::tab_press(command);
			return;
		}

		/* Check if enter was pressed. If so swap out
		 * command with prev_cmd_holder string. */
		if (keypress == 10) {
			check_return = 10;
			command = prev_cmd_holder;
			return;
		}

		if (keypress == 127) {
			Blueshell::backspace(prev_cmd_holder);
			Blueshell::print_line(prev_cmd_holder);
			continue;
		}

		if (keypress != 91 && keypress != 27) {
			command.push_back(keypress);
			return;
		}

		/* If none of the above, then check for arrow keys */
		if (keypress == 91) {
			keypress = Blueshell::getch();
		}

		switch (keypress) {
			// If up arrow was pressed:
			case 65: {
				if (!previous_commands.empty() &&
					vec_size < previous_commands.size()) {
					/* If prev_cmd_holder matches current position in
					 *  previous_commands vector, print next element */
					prev_cmd_holder =
						(prev_cmd_holder == previous_commands[vec_size].second)
							? (vec_size <= previous_commands.size() - 2)
								  ? previous_commands[vec_size + 1].second
								  : previous_commands[vec_size].second
							: previous_commands[vec_size].second;

					Blueshell::print_line(prev_cmd_holder);

					(vec_size >= previous_commands.size() - 1)
						? vec_size = previous_commands.size() - 1
						: ++vec_size;
				}
				continue;
			}

			// If down arrow was pressed:
			case 66: {
				/* If the down arrow was pressed, and are at the
				 *  start of previous_commands, print out any previously
				 *  typed chars if available */
				if (vec_size == 0) {
					Blueshell::print_line(command);
					prev_cmd_holder = command;
				}

				/* Start traversing over the previous_commands vector
				 *  to show any commands available. */
				if (vec_size >= 1) {
					/* If prev_cmd_holder matches current postion in
					 *  previous_commands vector, either print previous element
					 *  or if 0, print 'command' to display anything
					 *  previously typed before using arrow keys. */
					prev_cmd_holder =
						(prev_cmd_holder ==
								 previous_commands[vec_size - 1].second
							 ? ((vec_size == 1)
									? command
									: previous_commands[vec_size - 2].second)
							 : previous_commands[vec_size - 1].second);

					Blueshell::print_line(prev_cmd_holder);
					//                 }

					(vec_size <= 1) ? vec_size = 0 : --vec_size;
				}
				continue;
			}

			case 67: {
				channel << "\x1b[1C" << IOCtrl::end;
				continue;
			}

			case 68: {
				//			    std::string check_position{"\x1b[6n"};
				//			    std::cout<<check_position<<'\n';

				//                if(strncmp("\x1b[6n","buttface", 3))
				//                std::cout<<"eh there";

				//			    (check_position.find("butt"))?
				//std::cout<<"Found"
				//: std::cout<<"Not found"; 			    std::cout
				// <<check_position;

				//			    if( check_position.find("4R")){
				//                    std::cout<<"Beginning reached\n";
				//			    }
				channel << "\x1b[1D" << IOCtrl::end;
				std::cout << "\x1b[6n" << '\n';
				continue;
			}
		}
	}
	return;
}
