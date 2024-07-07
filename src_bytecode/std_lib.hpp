#ifndef STD_LIB_HPP
#define STD_LIB_HPP

#include <string>
#include <vector>

// array of the function names
const std::vector<std::string> STD_LIB_FUNCTION_NAMES = {"test", "inc",     // test functions       
                                                         "str_concat", "str_substr", "str_len", "char_at", "replace_char" // string functions
                                                        };         

void std_lib_error(const std::string &function, const std::string &error){
    std::cerr << "Error in function " << function << ": " << error << std::endl;

    exit(1); // not the best way to handle errors, but it's fine for now
}

// test functions --------------------------------------------
double test(){
    return 3.14;
}

double inc(double x){
    return x + 1;
}
// ------------------------------------------------------------

// string functions ------------------------------------------
std::string str_concat(std::string a, std::string b){
    return a + b;
}

std::string str_substr(std::string a, int start, int length){ // start is 0-based, 
                                                              // if start + length is greater than the length of the string,
                                                              // it will return the substring from start to the end of the string
    return a.substr(start, length);
}

int str_len(std::string a){
    return a.length();
}

std::string char_at(std::string a, int index){ // index is 0-based
    if(index < 0 || index >= a.length()){
        std_lib_error("char_at", "index [" + std::to_string(index) + "] out of bounds");
    }

    return std::string(1, a[index]);
}

std::string replace_char(std::string a, int index, std::string c){ // index is 0-based
    if(index < 0 || index >= a.length()){
        std_lib_error("replace_char", "index [" + std::to_string(index) + "] out of bounds");
    }
    a[index] = c[0];
    return a;
}

#endif // STD_LIB_HPP