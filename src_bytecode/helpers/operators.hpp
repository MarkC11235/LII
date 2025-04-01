#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include <map>
#include <string>
#include <tuple>

// Remove extern and make it a function that returns the map
std::map<std::string, std::tuple<int, std::string>> get_operators();
bool is_binary_operator(std::string op);
bool is_unary_operator(std::string op);
bool is_access_operator(std::string op);
bool is_call_operator(std::string op);

#endif // OPERATORS_HPP