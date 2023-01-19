#ifndef CMD_MAP_HPP
#define CMD_MAP_HPP

#include <algorithm>
#include <deque>
#include <iostream>
#include <set>

using _register = std::function<int(std::deque<std::string>&)>;

/* Struct for the members needed in Cmd_map. Thanks to
 * Alipha from C++ basics for showing me how to fix
 * the error having a nested struct and creating
 * a hash for unordered_set issue. */
namespace details
{
struct func_info {
	std::string func_name = "noname";
	_register func_command;
	std::string short_desc = "nodesc";
	std::string long_desc = "nodesc";
	size_t number_of_args;
	size_t number_of_options;

	friend bool operator==(const func_info& first, const func_info& second)
	{
		return first.func_name == second.func_name;
	}

	friend bool operator<(const func_info& first, const func_info& second)
	{
		return first.func_name < second.func_name;
	}
};
}  // namespace details

// Creates a hash for func_info.
namespace std
{
template<> struct hash<details::func_info> {
	std::size_t operator()(details::func_info const& s) const noexcept
	{
		std::size_t first{std::hash<std::string>{}(s.func_name)};
		std::size_t second{std::hash<std::string>{}(s.short_desc)};
		return first ^ (second << 1);
	}
};
}  // namespace std

class Cmd_map
{
private:
	using func_info = details::func_info;
	using func_set = std::set<func_info>;

	/* std::set to store a collection of
	 * commands with the help descriptions. */
	func_set commands;

public:
	bool find_match(const std::string&);

	/* Check if number of arguments sent match the
	 * number of arguments required. */
	bool match_args(const std::string&, size_t);

	/*  Function to add the commands to the 'command'
	 * container. */
	void add_command(const std::string&,
					 _register&,
					 const std::string&,
					 const std::string&,
					 size_t,
					 size_t);

	/* Return a container of func_info struct
	 * to access the members. */
	func_info send_element(const std::string&);

	/* Function to send all short descriptions of the
	 * commands available. */
	std::vector<std::pair<std::string, std::string>> short_help();

	/* Function to return list of commands user wants
	 * the longer descriptions of. */
	std::vector<std::pair<std::string, std::string>> long_help();

	/* Function to return reference to 'commands' for
	 * lookups. */
	const func_set& get_set();
};

#endif  // CMD_MAP_HPP
