#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <string>
#include <vector>
#include <iostream>

#include "helpers.hpp"

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