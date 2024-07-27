#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <map>
#include <vector>
#include <string>
#include <cstdint>

typedef int8_t CODE_SIZE; // Bytecode size, only 8 bits for pointers will be too small for large programs

struct function; // Forward declaration

struct function {
    CODE_SIZE* code; // Bytecode array
    int count;
    int capacity;

    std::string name; // Function name

    //function* parent; // Parent function
    
    //arguments
    std::vector<std::string> arguments;

    //std::map<std::string, function*> functions; // function map
};

#endif //FUNCTION_HPP