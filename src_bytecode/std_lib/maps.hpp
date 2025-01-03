#ifndef MAPS_HPP
#define MAPS_HPP

#include <map>
#include <string>
#include <vector>

#include "../Value.hpp"

/*
Returns the number of key-value pairs in the map
*/
int map_size(std::map<std::string, Value> map){
    return map.size();
}

/*
Combines two maps into one
If a key is in both maps, the value from the second map is used
*/
std::map<std::string, Value> map_join(std::map<std::string, Value> a, std::map<std::string, Value> b){
    for(auto it = b.begin(); it != b.end(); it++){
        a[it->first] = it->second;
    }
    return a;
}


#endif // MAPS_HPP