#include "../include/iosqueak/blueshell/blueshell.hpp"

Alias::arguments Blueshell::process_options(std::string& sent_command)
{
	Alias::arguments options;

	std::string word;
	for (size_t ch{0}; ch < sent_command.size(); ++ch) {
		// If there is no " or space, add letter to word.
		if (sent_command[ch] != '"' && sent_command[ch] != '\'' &&
			sent_command[ch] != ' ') {
			word.push_back(sent_command[ch]);
			continue;
		}
		/* If there is a " or ', find closing quote and check
		 * for " or ' in between quotes.
		 */

		if (sent_command[ch] == '"' || sent_command[ch] == '\'') {
			// Check if incrementing 'ch' will cause buffer overrun.
			int plus1{((ch + 1) < sent_command.size()) ? 1 : 0};
			int plus2{((ch + 2) < sent_command.size())   ? 2
					  : ((ch + 1) < sent_command.size()) ? 1
														 : 0};

			if (ch + 1 < sent_command.size())
				++ch;

			while (sent_command[ch] != '"' && sent_command[ch] != '\'' &&
				   (ch + 1) < sent_command.size()) {
				if (sent_command[ch] == '\\' &&
					sent_command[ch + plus1] == '\\') {
					word.push_back(sent_command[ch + plus1]);
					ch += plus2;
				}
				if (sent_command[ch] == '\\' &&
					(sent_command[ch + plus1] == '"' ||
					 sent_command[ch + plus1] == '\'')) {
					// Insert " or ' into the token.
					word.push_back(sent_command[ch + plus1]);
					ch += plus2;
					continue;
				}
				if (ch == sent_command.size() - 1) {
					word.push_back(sent_command[ch]);
				}
				word.push_back(sent_command[ch]);
				if (ch + 1 < sent_command.size())
					++ch;
			}
			// Increment to skip closing ".
			if (ch + 1 < sent_command.size())
				++ch;
		}

		if (sent_command[ch] == ' ') {
			options.push_back(word);
			word = std::string();
		}
	}
	// Add final word to deque.
	options.push_back(word);

	/* Remove actual command from the deque.
	 * This will leave only the options/flags.
	 */
	options.pop_front();

	return options;
}
