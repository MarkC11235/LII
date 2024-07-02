#ifndef STD_LIB_HPP
#define STD_LIB_HPP

#include <string>
#include <vector>

// array of the function names
const std::vector<std::string> STD_LIB_FUNCTION_NAMES = {"test", "inc",     // test functions       
                                                         "str_concat", "str_substr", "str_len" // string functions
                                                        };         

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

#endif // STD_LIB_HPP