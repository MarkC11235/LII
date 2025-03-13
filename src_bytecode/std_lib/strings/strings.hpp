#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <string>
#include <vector>
#include <iostream>

#include "../std_lib_helpers/helpers.hpp"

// THESE COLORS HAVE TO BE DEFINED IN THE SAME HEADER AS THEY ARE USED
// if not, the compiler gets angry
#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define YELLOW_TEXT "\033[1;33m"
#define BLUE_TEXT "\033[1;34m"
#define WHITE_TEXT "\033[1;37m"

#define RESET_TEXT "\033[0m"

int print_colored_text(std::string text, std::string color);
std::string string_join(std::string a, std::string b);
std::string string_substring(std::string a, int start, int length);
int string_length(std::string a);
std::string char_at(std::string a, int index);
std::string replace_char(std::string a, int index, std::string c);
std::vector<Value> string_to_vector(std::string a);
std::vector<Value> string_split(std::string a, std::string delimiter);

#endif // STRINGS_HPP