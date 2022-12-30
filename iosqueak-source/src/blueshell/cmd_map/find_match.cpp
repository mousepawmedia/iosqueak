#include "iosqueak/cmd_map.hpp"

bool Cmd_map::find_match(const std::string& sent_command)
{
	return std::any_of(commands.begin(),
					   commands.end(),
					   [&sent_command](const auto& cmd) {
						   return cmd.func_name == sent_command;
					   });
} 
