#ifndef ALIAS_HPP
#define ALIAS_HPP

#include <algorithm>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

class Alias
{
public:
    using arguments = std::deque<std::string>;
    using _register = std::function<int(arguments&)>;
};
#endif // ALIAS_HPP
