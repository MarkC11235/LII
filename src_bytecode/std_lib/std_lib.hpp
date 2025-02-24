#ifndef STD_LIB_HPP
#define STD_LIB_HPP

#include <string>
#include <vector>
#include <iostream>
#include <variant>
#include <functional>
#include <any>
#include <utility>

#include "./std_lib_helpers/helpers.hpp"
#include "./strings/strings.hpp"
#include "./vectors/vectors.hpp"
#include "./files/files.hpp"
#include "./graphics/graphics.hpp"
#include "./misc/misc.hpp"
#include "./http/http_utils.hpp"
#include "./maps/maps.hpp"
#include "./matrix/matrix.hpp"
#include "./math/math.hpp"

typedef std::function<std::any(std::vector<std::any>, std::vector<std::string>)> STD_LIB_FUNCTION;

struct STD_LIB_FUNCTION_INFO {
    std::string name;
    STD_LIB_FUNCTION function;
    std::string return_type;
    std::vector<std::string> arg_types;
};

template<typename Return, typename... Args>
STD_LIB_FUNCTION make_std_lib_function(Return (*function)(Args...));

// Function declarations
void print_std_lib_function(const STD_LIB_FUNCTION_INFO &func);
bool LII_type_matches_cpp_type(Value value, std::string type);
bool any_type_check(std::any value, std::string type);
std::any cast_LII_type_to_cpp_type(Value value, std::string type);
bool is_correct_number_of_parameters(std::string std_lib_name, int num_params);

// Test function declarations
int do_nothing();
double test();
double inc(double a);

// External declarations
extern const std::vector<STD_LIB_FUNCTION_INFO> STD_LIB_FUNCTIONS_DEFINITIONS;

#endif // STD_LIB_HPP