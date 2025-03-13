#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include <map>
#include <string>
#include <tuple>

// Remove extern and make it a function that returns the map
std::map<std::string, std::tuple<int, std::string>> get_operators();

#endif // OPERATORS_HPP