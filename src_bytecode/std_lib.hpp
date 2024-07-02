#ifndef STD_LIB_HPP
#define STD_LIB_HPP

#include <string>
#include <vector>

// array of the function names
const std::vector<std::string> STD_LIB_FUNCTION_NAMES = {"test", "inc"};

double test(){
    return 3.14;
}

double inc(double x){
    return x + 1;
}

#endif // STD_LIB_HPP