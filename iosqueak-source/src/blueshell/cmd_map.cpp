#include "iosqueak/cmd_map.hpp"

bool Cmd_map::find_match(const std::string& sent_command)
{
	return std::any_of(commands.begin(),
					   commands.end(),
					   [&sent_command](const auto& cmd) {
						   return cmd.func_name == sent_command;
					   });
}

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
		return check->number_of_args == sent_args;
	}
	return false;
}

void Cmd_map::add_command(const std::string& sent_name,
						  _register& sent_command,
						  const std::string& short_desc,
						  const std::string& long_desc,
						  size_t number_of_args)
{
	commands.insert({func_info{sent_name,
							   sent_command,
							   short_desc,
							   long_desc,
							   number_of_args}});
}

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

/* Function to send all short descriptions of the
 * commands available. */
std::vector<std::pair<std::string, std::string>> Cmd_map::short_help()
{
	std::vector<std::pair<std::string, std::string>> vec;
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
	std::transform(this->commands.begin(),
				   this->commands.end(),
				   std::back_inserter(vec),
				   [](auto& cmd) {
					   return std::pair{cmd.func_name, cmd.long_desc};
				   });

	return vec;
}
