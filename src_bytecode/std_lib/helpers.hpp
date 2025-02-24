#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>
#include <vector>
#include <iostream>

#include "../helpers/Value.hpp"

#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define YELLOW_TEXT "\033[1;33m"
#define BLUE_TEXT "\033[1;34m"
#define WHITE_TEXT "\033[1;37m"

#define RESET_TEXT "\033[0m"

/*
To be used in the standard library functions to print errors
*/
void std_lib_error(const std::string &function, const std::string &error){
    std::cout << RED_TEXT;
    std::cerr << "Error in function " << function << ": " << error << std::endl;
    std::cout << RESET_TEXT;

    exit(1); // TODO: not the best way to handle errors, but it's fine for now
}


#endif // HELPERS_HPP