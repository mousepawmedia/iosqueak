#include "../include/iosqueak/blueshell.hpp"

// Create the initial shell when creating a Blueshell object.
void Blueshell::initial_shell()
{
	// Register the default commands.
	Blueshell::registerdefaults();

	Blueshell::clear_screen(empty_container);

	channel << IOFormatTextBG::black << IOFormatTextFG::green
			<< "Now running in " << shell_name << " shell." << IOCtrl::endl;

	// Keep looping until 'exit' or 'quit' is typed in, then break out of loop.
	while (true) {
		// Variable to store returned key press.
		size_t returned_key{0};

		// Keep looping for key presses until 'enter' is pressed.
		while (true) {
			Blueshell::print_line(check_command);
			if (check_command == "!")
				returned_key = Blueshell::bang(check_command);

			// Variable to check if returned function sent 'keypress'.
			size_t keypress{(returned_key) ? returned_key : Blueshell::getch()};

			// Check for the key pressed.
			switch (keypress) {
				/* Check if backspace was pressed. This can
				 * be different on different os's I think.
				 */
				case 8: {
					Blueshell::backspace(check_command);
					Blueshell::print_line(check_command);
					returned_key = 0;
					continue;
				}

				// Check if 'tab' was pressed.
				case 9: {
					returned_key = Blueshell::tab_press(check_command);
					continue;
				}

				// If enter was pressed, break loop.
				case 10: {
					// Check for closed quotes in string.
					Blueshell::check_quote(check_command);
					break;
				}

				// Check if arrow key was pressed.
				case 27: {
					returned_key = Blueshell::arrow_press(check_command);
					continue;
				}

				// Check if '!' was pressed.
				case 33: {
					/* Check if ! is not at the start of command,
					 * and if so, do not run bang command.
					 */
					if (check_command.size() > 0) {
						Blueshell::insert_char(check_command, keypress);
						continue;
					}
					// Insert ! and send command to bang function.
					Blueshell::insert_char(check_command, keypress);
					returned_key = Blueshell::bang(check_command);
					continue;
				}

				// Check if backspace was pressed.
				case 127: {
					Blueshell::backspace(check_command);
					Blueshell::print_line(check_command);
					returned_key = 0;
					continue;
				}

				/* If none of the above, insert pressed key
				 *  into command string */
				default: {
					Blueshell::insert_char(check_command, keypress);
					returned_key = 0;
					continue;
				}
			}

			break;
		}
		/* Check if a previous command was selected. Use
		 *  that if it was, otherwise use any previously
		 *  typed command. */
		command = (!prev_cmd_holder.empty() ? prev_cmd_holder : check_command);

		// If sent command is to exit or quit shell.
		if (command == "quit" || command == "exit") {
			channel << "\nLeaving " << shell_name << " shell." << IOCtrl::endl;
			return;
		}

		// Process the command to break into strings if options are provided.
		Blueshell::process_command(command);

		// Reset commands to empty string.
		check_command = std::string();
		command = std::string();
		prev_cmd_holder = std::string();

		// Reset integers to default.
		cursor_moves = 0;
		vec_size = 0;
		returned_key = 0;
	}
}
