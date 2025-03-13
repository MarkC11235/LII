#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <map>
#include <vector>
#include <string>
#include <cstdint>

typedef int16_t CODE_SIZE; // Bytecode size, only 8 bits for pointers will be too small for large programs

std::string CODE_TO_NUMBER_STRING(CODE_SIZE code);

/*
Defines a function object, which contains the bytecode array, the arguments, and jit information
*/
struct function {
    CODE_SIZE* code; // Bytecode array
    int count;
    int capacity;

    // Vector of flagged instruction indices
    std::vector<std::tuple<int, std::string>> flags;

    std::string name; // Function name
    
    //arguments
    std::vector<std::string> arguments;

    //for jit compilation
    int times_called = 0;
    int jit_index = -1;
};

#endif //FUNCTION_HPP