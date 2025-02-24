#include "strings.hpp"

/*
Prints the correct escape sequence to print colored text
Throws an error if the color is not recognized
*/
int print_colored_text(std::string text, std::string color){
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
        return 1;
    }

    std::cout << text << std::endl << RESET_TEXT;
    return 0;
}

/*
Returns a string that is the concatenation of the two strings
*/
std::string string_join(std::string a, std::string b){
    return a + b;
}

/*
Returns a substring of the given string
If start + length is greater than the length of the string, it will return the substring from start to the end of the string
*/
std::string string_substring(std::string a, int start, int length){ 
    return a.substr(start, length);
}

/*
Returns the length of the given string
*/
int string_length(std::string a){
    return a.length();
}

/*
returns the character (string, length 1) at the given index
*/
std::string char_at(std::string a, int index){ 
    if(index < 0 || (unsigned)index >= a.length()){
        std_lib_error("char_at", "index [" + std::to_string(index) + "] out of bounds");
    }

    return std::string(1, a[index]);
}

/*
Replaces the character (string, length 1) at the given index with the given character (string, length 1) and returns the new string
If a string with more than one character is passed, only the first character will be used
*/
std::string replace_char(std::string a, int index, std::string c){ 
    if(index < 0 || (unsigned)index >= a.length()){
        std_lib_error("replace_char", "index [" + std::to_string(index) + "] out of bounds");
    }
    a[index] = c[0];
    return a;
}

/*
Converts a string to a vector of strings, where each string is a character from the original string
*/
std::vector<Value> string_to_vector(std::string a){
    std::vector<Value> v;
    for(int i = 0; i < (int)a.length(); i++){
        v.push_back({Value_Type::STRING, std::string(1, a[i])});
    }
    return v;
}

/*
Splits a string into a vector of strings based on a delimiter
*/
std::vector<Value> string_split(std::string a, std::string delimiter){
    std::vector<Value> v;
    size_t pos = 0;
    std::string token;
    while ((pos = a.find(delimiter)) != std::string::npos) {
        token = a.substr(0, pos);
        v.push_back({Value_Type::STRING, token});
        a.erase(0, pos + delimiter.length());
    }
    v.push_back({Value_Type::STRING, a});
    return v;
}
