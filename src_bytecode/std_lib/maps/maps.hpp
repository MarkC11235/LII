#ifndef MAPS_HPP
#define MAPS_HPP

#include <map>
#include <string>
#include <vector>
#include "../../helpers/Value.hpp"

int map_size(std::map<std::string, Value> map);

std::map<std::string, Value> map_join(std::map<std::string, Value> a, std::map<std::string, Value> b);

#endif // MAPS_HPP