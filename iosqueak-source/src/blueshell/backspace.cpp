#include "../include/iosqueak/blueshell.hpp"

size_t Blueshell::backspace(std::string& sent_command)
{
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
