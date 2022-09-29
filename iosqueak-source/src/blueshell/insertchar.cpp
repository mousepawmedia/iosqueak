#include "../include/iosqueak/blueshell.hpp"

void Blueshell::insert_char(std::string& sent_command, int keypress)
{
	if (cursor_moves <= static_cast<int>(sent_command.size())) {
		sent_command.insert(sent_command.size() - cursor_moves,
							1,
							static_cast<char>(keypress));
	}
}
