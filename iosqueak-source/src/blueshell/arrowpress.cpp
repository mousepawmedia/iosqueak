#include "../include/iosqueak/blueshell.hpp"

size_t Blueshell::arrow_press(std::string& sent_command)
{
	while (true) {
		int keypress{Blueshell::getch()};

		/* Sometimes this is required for operating
		 * systems that send different code for
		 * delete button
		 */
		if (keypress == 126) {
			Blueshell::delete_char(sent_command);
			return 0;
		}
		/* If arrow keys are not pressed, return
		 * which key was. */
		if (keypress != 27 && keypress != 91) {
			/* If no previous command was selected
			 * return keypress. */
			if (prev_cmd_holder.empty())
				return keypress;

			/* If a previous command was selected
			 * send that back to caller and empty
			 * prev_cmd_holder. */
			if (prev_cmd_holder != sent_command) {
				sent_command = prev_cmd_holder;
				prev_cmd_holder = std::string();

				return keypress;
			}
		}

		keypress = Blueshell::getch();

		if (keypress == 91) {
			keypress = Blueshell::getch();
		}

		switch (keypress) {
			// If up arrow was pressed:
			case 65: {
				(vec_size >= previous_commands.size())
					? vec_size = previous_commands.size()
					: ++vec_size;

				cursor_moves = 0;
				if (!previous_commands.empty() &&
					vec_size <= previous_commands.size()) {
					/* If prev_cmd_holder matches current position in
					 *  previous_commands vector, print next element */
					prev_cmd_holder = previous_commands[vec_size - 1].second;

					Blueshell::print_line(prev_cmd_holder);
				}
				continue;
			}

			// If down arrow was pressed:
			case 66: {
				cursor_moves = 0;
				(vec_size <= 1) ? vec_size = 0 : --vec_size;

				/* If the down arrow was pressed and at the start
				 *  of previous_commands, print out any previously
				 *  typed chars if available */
				if (vec_size == 0) {
					Blueshell::print_line(sent_command);
					prev_cmd_holder = std::string();
				}

				/* Start traversing over the previous_commands vector
				 *  to show any commands available. */
				if (vec_size >= 1) {
					/* If prev_cmd_holder matches current postion in
					 *  previous_commands vector, either print previous element
					 *  or if 0, print 'command' to display anything
					 *  previously typed before using arrow keys. */
					prev_cmd_holder = previous_commands[vec_size - 1].second;
					Blueshell::print_line(prev_cmd_holder);
				}
				continue;
			}

			// Check for right arrow press.
			case 67: {
				if (cursor_moves > 0) {
					--cursor_moves;
					Blueshell::print_line((!prev_cmd_holder.empty())
											  ? prev_cmd_holder
											  : sent_command);
				}

				continue;
			}

			// Check for left arrow press.
			case 68: {
				//
				if (cursor_moves <
					static_cast<int>((!prev_cmd_holder.empty())
										 ? prev_cmd_holder.size()
										 : sent_command.size())) {
					++cursor_moves;
					Blueshell::print_line((!prev_cmd_holder.empty())
											  ? prev_cmd_holder
											  : sent_command);
				}

				continue;
			}
		}
	}

	return 0;
}
