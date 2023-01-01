#include "iosqueak/cmd_map.hpp"

/*  Function to add the commands to the 'command'
 * container. */
void Cmd_map::add_command(const std::string& sent_name,
						  _register& sent_command,
						  const std::string& short_desc,
						  const std::string& long_desc,
						  size_t number_of_args,                          
						  size_t number_of_options
                         )
{
	commands.insert({func_info{sent_name,
							   sent_command,
							   short_desc,
							   long_desc,
							   number_of_args,
							   number_of_options}});
}
