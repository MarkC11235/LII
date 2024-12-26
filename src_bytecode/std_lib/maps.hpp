#ifndef MAPS_HPP
#define MAPS_HPP

#include <map>
#include <string>
#include <vector>

#include "../Value.hpp"

int map_size(std::map<std::string, Value> map){
    return map.size();
}


#endif // MAPS_HPP