
#include "../include/iosqueak/blueshell/blueshell.hpp"

// A function to show what commands are available.
int Blueshell::help(Alias::arguments& search_word)
{
	if (search_word.empty()) {
		channel << IOFormatTextFG::green << IOCtrl::n;
		channel << "****************************************" << IOCtrl::n;
		channel << "*                 Help                 *" << IOCtrl::n;
		channel << "* The following commands are available *" << IOCtrl::n;
		channel << "****************************************" << IOCtrl::n;

		channel << IOFormatTextFG::white << "\nquit " << IOFormatTextFG::green
				<< "or " << IOFormatTextFG::white << "exit "
				<< IOFormatTextFG::green << "to leave " << shell_name
				<< " shell" << IOCtrl::n << IOCtrl::endl;

		for (auto& cmd : stored_commands.short_help()) {
			// Set the command to white, and the description to green.
			std::cout << "\033[37m" << std::setw(20) << std::left << cmd.first
					  << "\033[38;2;0;255;0m";
			Blueshell::print_string(cmd.second);
			std::cout << "\n\n";
		}
		std::cout << "\nUse help <command name> for a more indepth description "
					 "of the command (eg 'help history')\n"
				  << "\033[37m";
		std::cout << "\033[37m";

	} else {
		channel << IOFormatTextFG::green << IOCtrl::n;
		channel << "****************************************" << IOCtrl::n;
		channel << "*       Help for the command(s):       *" << IOCtrl::n;
		channel << "****************************************" << IOCtrl::n
				<< IOCtrl::end;
		for (auto& word : search_word) {
			for (auto& cmd : stored_commands.long_help()) {
				if (word == cmd.first) {
					std::cout << "\033[37m" << std::setw(20) << std::left
							  << cmd.first << "\033[38;2;0;255;0m";
					Blueshell::print_string(cmd.second);
					std::cout << "\n\n";
				}
			}
		}
		channel << IOFormatTextFG::white << IOCtrl::end;
	}

	return 0;
}

void Blueshell::print_string(const std::string& str)
{
	std::stringstream stream(str);
	size_t column{40}, count{column};

	/* Go through description and print out each
	 * word. If the line is too long, wrap the line
	 */
	while (!stream.eof()) {
		std::string word;
		std::getline(stream, word, ' ');
		if (word.size() < count) {
			count -= word.size();
			std::cout << word << ' ';
		} else {
			count = column;
			std::cout << std::setfill(' ') << std::setw(21) << std::left << '\n'
					  << word << ' ';
			count -= word.size();
		}
	}
}
