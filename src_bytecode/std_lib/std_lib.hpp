#ifndef STD_LIB_HPP
#define STD_LIB_HPP

#include <string>
#include <vector>
#include <iostream>

#include "strings.hpp" // include the string functions

// array of the function names
const std::vector<std::string> STD_LIB_FUNCTION_NAMES = {"test", "inc",     // test functions       
                                                         "str_concat", "str_substr", "str_len", "char_at", "replace_char", // string functions
                                                         "print_colored_text" // custom functions
                                                        };         

// test functions --------------------------------------------
double test(){
    return 3.14;
}

double inc(double x){
    return x + 1;
}
// ------------------------------------------------------------

void print_colored_text(const std::string &text, const std::string &color){
    if(color == "red"){
        std::cout << RED_TEXT;
    }else if(color == "green"){
        std::cout << GREEN_TEXT;
    }else if(color == "yellow"){
        std::cout << YELLOW_TEXT;
    }else if(color == "blue"){
        std::cout << BLUE_TEXT;
    }else if(color == "white"){
        std::cout << WHITE_TEXT;
    }else{
        std_lib_error("print_colored_text", "invalid color [" + color + "]");
    }

    std::cout << text << std::endl;
    std::cout << RESET_TEXT;
}

#endif // STD_LIB_HPP