#include "../include/iosqueak/blueshell.hpp"

void Blueshell::process_options(std::string& sent_command)
{
	std::deque<std::string> options;

	std::string word;
	for (size_t ch{0}; ch <= sent_command.size(); ++ch) {
		// If there is no " or space, add letter to word.
		if (sent_command[ch] != '"' && sent_command[ch] != ' ') {
			word.push_back(sent_command[ch]);
			continue;
		}
		/* If there is a ", find closing quote and check
		 * for ' in between quotes.
		 */
		if (sent_command[ch] == '"') {
			++ch;
			while (sent_command[ch] != '"') {
				if (sent_command[ch] == '\\' && sent_command[ch + 1] == '\\') {
					word.push_back(sent_command[ch + 1]);
					ch += 2;
				}
				if (sent_command[ch] == '\\' &&
					(sent_command[ch + 1] == '"' ||
					 sent_command[ch + 1] == '\'')) {
					// Insert " or ' into the token.
					word.push_back(sent_command[ch + 1]);
					ch += 2;
					continue;
				}
				word.push_back(sent_command[ch]);
				++ch;
			}
			// Increment to skip closing ".
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
	// For testing only. Print out each token.
	for (auto& token : options)
		std::cout << token << '\n';
}
