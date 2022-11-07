#include "../include/iosqueak/blueshell.hpp"

size_t Blueshell::backspace(std::string& sent_command)
{
	/* Two variables that will check if there is a backslash before
	 * the ' or ". If there is, add to inner ' or ". The second
	 * one will check for the ' or ".
	 */
	size_t backslash_check{
		(sent_command.size() > 1)
			? ((sent_command.size() - 2) - cursor_moves)
			: ((sent_command.size() - 1) + cursor_moves + 1)};
	size_t quote_check{(sent_command.size() > 1)
						   ? ((sent_command.size() - 1) - cursor_moves)
						   : ((sent_command.size()) + cursor_moves + 1)};

	// Check if there is a nested string (\' or \").
	if (sent_command[backslash_check] == '\\' &&
		(sent_command[quote_check] == '\"' ||
		 sent_command[quote_check] == '\'')) {
		if (sent_command[quote_check] == '\"') {
			inner_quote = !inner_quote;
		} else if (sent_command[quote_check] == '\'') {
			inner_single = !inner_single;
		}
	}
	// If not a nested quote, change outer true or false.
	else if (sent_command[quote_check] == '\"' ||
			 sent_command[quote_check] == '\'') {
		if (sent_command[quote_check] == '\"') {
			outer_quote = !outer_quote;
		} else if (sent_command[quote_check] == '\'') {
			outer_single = !outer_single;
		}
	}
	/* If using the arrow keys to move around, and then erasing
	 * the first character of the command, erase the first char.
	 * If not here, there is an overflow error
	 */
	if (cursor_moves == static_cast<int>(sent_command.size())) {
		sent_command.erase(cursor_moves, 1);
	} else {
		/* Erase characters from the string based on location
		 * of the cursor.
		 */
		sent_command.erase((sent_command.size() - 1) - cursor_moves, 1);
	}

	return 0;
}
