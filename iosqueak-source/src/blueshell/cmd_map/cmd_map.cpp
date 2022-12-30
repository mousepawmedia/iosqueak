#include "iosqueak/cmd_map.hpp"

using func_set = std::set<details::func_info>;
const func_set& Cmd_map::get_set() { return commands; }
