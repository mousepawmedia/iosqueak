#include "iosqueak/cmd_map.hpp"

/* Return a container of func_info struct
* to access the members. */
details::func_info Cmd_map::send_element(const std::string& command)
{
	auto name_check{
		std::find_if(commands.begin(), commands.end(), [&command](auto& cmd) {
			return cmd.func_name == command;
		})};
	func_info send;
	if (name_check != commands.end()) {
		send = *name_check;
	}
	return send;
} 
