#include "iosqueak/cmd_map.hpp"

/* Function to send all short descriptions of the
 * commands available. */
std::vector<std::pair<std::string, std::string>> Cmd_map::short_help()
{
	std::vector<std::pair<std::string, std::string>> vec;
    
    // Reserve space for 50 commands. Saves reallocation overhead.
    vec.reserve(50);
	std::transform(this->commands.begin(),
				   this->commands.end(),
				   std::back_inserter(vec),
				   [](auto& cmd) {
					   return std::pair{cmd.func_name, cmd.short_desc};
				   });

	return vec;
}

/* Function to return list of commands user wants
 * the longer descriptions of. */
std::vector<std::pair<std::string, std::string>> Cmd_map::long_help()
{
	std::vector<std::pair<std::string, std::string>> vec;
    // Reserve space for 5 commands. Saves reallocation overhead.
    vec.reserve(5);
	std::transform(this->commands.begin(),
				   this->commands.end(),
				   std::back_inserter(vec),
				   [](auto& cmd) {
					   return std::pair{cmd.func_name, cmd.long_desc};
				   });

	return vec;
}
