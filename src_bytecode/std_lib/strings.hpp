#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <string>
#include <vector>
#include <iostream>

#include "helpers.hpp"

// THESE COLORS HAVE TO BE DEFINED IN THE SAME HEADER AS THEY ARE USED
// if not, the compiler gets angry
#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define YELLOW_TEXT "\033[1;33m"
#define BLUE_TEXT "\033[1;34m"
#define WHITE_TEXT "\033[1;37m"

#define RESET_TEXT "\033[0m"

void print_colored_text(std::string text, std::string color){
    if(color == "red"){
        std::cout << RED_TEXT; 
    } else if(color == "green"){
        std::cout << GREEN_TEXT;
    } else if(color == "yellow"){
        std::cout << YELLOW_TEXT;
    } else if(color == "blue"){
        std::cout << BLUE_TEXT;
    } else if(color == "white"){
        std::cout << WHITE_TEXT;
    } else {
        std_lib_error("print_colored_text", "unknown color [" + color + "]");
    }

    std::cout << text << std::endl << RESET_TEXT;
}

std::string str_concat(std::string a, std::string b){
    return a + b;
}

// start is 0-based, 
// if start + length is greater than the length of the string, it will return the substring from start to the end of the string
std::string str_substr(std::string a, int start, int length){ 
    return a.substr(start, length);
}

// returns the length of the string
int str_len(std::string a){
    return a.length();
}

// index is 0-based
// returns the character (right now string because no char type) at the given index
std::string char_at(std::string a, int index){ 
    if(index < 0 || (unsigned)index >= a.length()){
        std_lib_error("char_at", "index [" + std::to_string(index) + "] out of bounds");
    }

    return std::string(1, a[index]);
}

// index is 0-based
// replaces the character (string) at the given index with the given character (string)
// if a string with more than one character is passed, only the first character will be used
std::string replace_char(std::string a, int index, std::string c){ 
    if(index < 0 || (unsigned)index >= a.length()){
        std_lib_error("replace_char", "index [" + std::to_string(index) + "] out of bounds");
    }
    a[index] = c[0];
    return a;
}


#endif // STRINGS_HPP