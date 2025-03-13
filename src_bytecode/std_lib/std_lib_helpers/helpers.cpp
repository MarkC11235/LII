#include "helpers.hpp"

/*
To be used in the standard library functions to print errors
*/
void std_lib_error(const std::string &function, const std::string &error){
    std::cout << RED_TEXT;
    std::cerr << "Error in function " << function << ": " << error << std::endl;
    std::cout << RESET_TEXT;

    exit(1); // TODO: not the best way to handle errors, but it's fine for now
}
