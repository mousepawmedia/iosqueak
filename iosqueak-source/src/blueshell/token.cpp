#include "../include/iosqueak/blueshell/blueshell.hpp"

Alias::arguments Blueshell::tokens(const std::string& sent_command)
{
	std::stringstream stream(sent_command);
	Alias::arguments tokens;

	while (!stream.eof()) {
		std::string ss;
		stream >> ss;
		tokens.push_back(ss);
	}
	return tokens;
}
