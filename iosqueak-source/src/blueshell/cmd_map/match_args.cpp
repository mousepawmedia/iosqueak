#include "iosqueak/cmd_map.hpp"

/* Check if number of arguments sent match the
 * number of arguments required. */
bool Cmd_map::match_args(const std::string& cmd_name, size_t sent_args)
{
	auto check{std::find_if(commands.begin(),
							commands.end(),
							[&cmd_name](const auto& cmd) {
								return cmd.func_name == cmd_name;
							})};
	if (check != commands.end()) {
		return (sent_args >= check->number_of_args) && sent_args <= (check->number_of_args + check->number_of_options);
	}
	return false;
}
