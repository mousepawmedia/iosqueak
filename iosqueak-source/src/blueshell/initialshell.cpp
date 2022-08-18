#include "../include/iosqueak/blueshell.hpp"

// Create the initial shell when creating a Blueshell object.
void Blueshell::initial_shell()
{
	Blueshell::clear_screen();

	// Delete when up/down arrow is working...
	previous_commands.push_front(std::pair(1, "Diamond"));
	previous_commands.push_front(std::pair(2, "test"));
	previous_commands.push_front(std::pair(3, "Crazy"));

	// Creates member function pointers to predefined commands.
	_register help_cmd{std::bind(&Blueshell::help, this, _1)};
	_register clear_cmd{std::bind(&Blueshell::clear_screen, this, _1)};
	_register history_cmd{std::bind(&Blueshell::history, this, _1)};

	// Stores the commands in the stored_commands map.
	stored_commands["help"] = {"This menu", help_cmd};
	stored_commands["clear"] = {"Clears the screen", clear_cmd};
	stored_commands["history"] = {"Shows previously entered commands",
								  history_cmd};

	channel << IOFormatTextBG::black << IOFormatTextFG::green
			<< "Now running in Blueshell." << IOCtrl::endl;

	// Delete after registering tests function is correct. Figure out how to
	// pass function parameter better.
	std::cout << "Registering test command\n";
	register_command("test", &Blueshell::test, "Just testing out my work");
	register_command("teat", &Blueshell::test, "Just testing out my work");

	// Keep looping until 'exit' or 'quit' is typed in, then break out of loop.
	while (true) {
		std::string command;

		// Keep looping for key presses until 'enter' is pressed.
		while (true) {
			// Returns cursor to beginning of line.
			Blueshell::print_line(command);

			// Variable to check key press.
			int keypress{Blueshell::getch()};

			// Print keypress to find ansi code
			//			            std::cout <<"Keypress: "<<keypress<<'\n';

			// Check for the key pressed.
			switch (keypress) {
				// Check if 'tab' was pressed.
				case 9: {
					Blueshell::tab_press(command);

					/* If 'enter' was pressed after arrow press. If
					 *  so, send command, otherwise check for keypress. */
					if (check_return == 10) {
						break;
					}
					continue;
				}

				// If enter was pressed, break loop.
				case 10: {
					break;
				}

				// Check if arrow key was pressed.
				case 27: {
					Blueshell::arrow_press(command);

					/* If 'enter' was pressed after arrow press. If
					 *  so, send command, otherwise check for keypress. */
					if (check_return == 10) {
						break;
					}
					continue;
				}

				case 33: {
					command.push_back(keypress);
					Blueshell::bang(command);
					break;
				}

				// Check if backspace was pressed.
				case 127: {
					Blueshell::backspace(command);
					continue;
				}

				/* If none of the above, insert pressed key
				 *  into command string */
				default: {
					command.push_back(keypress);
					continue;
				}
			}
			break;
		}

		std::stringstream ss(command);
		std::string first_command;
		ss >> first_command;

		std::string options{ss.str()};

		// Check if the command is available, if not send "Unknown command".
		if (first_command == "exit" || first_command == "quit") {
			channel << "\nLeaving Blueshell" << IOCtrl::endl;
			break;
		} else {
			// Checks if command is in stored commands.
			auto it{stored_commands.find(command)};
			if (it != stored_commands.end()) {
				(it->second.second)(first_command);
				/* Adds command to previous_commands container. If it is not
				 *  the last command added to the deque. */
				if (previous_commands.front().second != command) {
					previous_commands.push_front(
						std::pair((previous_commands.size() == 0)
									  ? 1
									  : previous_commands.front().first + 1,
								  command));
				}
			} else {
				channel << "\n"
						<< command << " Unknown command" << IOCtrl::endl;
			}
		}
	}
}
