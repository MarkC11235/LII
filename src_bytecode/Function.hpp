#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <map>
#include <vector>
#include <string>
#include <cstdint>

typedef int16_t CODE_SIZE; // Bytecode size, only 8 bits for pointers will be too small for large programs

std::string CODE_TO_NUMBER_STRING(CODE_SIZE code){
    return std::to_string((int)code);
}

struct function; // Forward declaration

struct function {
    CODE_SIZE* code; // Bytecode array
    int count;
    int capacity;

    // Vector of flagged instruction indices
    std::vector<std::tuple<int, std::string>> flags;

    std::string name; // Function name
    
    //arguments
    std::vector<std::string> arguments;
};

#endif //FUNCTION_HPP