#ifndef STD_LIB_HPP
#define STD_LIB_HPP

#include <string>
#include <vector>
#include <iostream>
#include <variant>
#include <functional>
#include <any>
#include <utility> // std::index_sequence, std::make_index_sequence

#include "strings.hpp" // include the string functions
#include "vectors.hpp" // include the vector functions
#include "files.hpp" 
#include "graphics.hpp"
#include "random.hpp"


//Allowed mappings of LII types to C++ types
// LII -> C++
// number -> double
// number -> int
// bool -> bool
// string -> std::string
// vector -> std::vector<Value>

typedef std::function<std::any(std::vector<std::any>, std::vector<std::string>)> STD_LIB_FUNCTION;

// array of the function pointers
struct STD_LIB_FUNCTION_INFO{
    std::string name;
    STD_LIB_FUNCTION function;
    std::string return_type; // C++ type
    std::vector<std::string> arg_types; // C++ types
};

template<typename Return, typename... Args>
STD_LIB_FUNCTION make_std_lib_function(Return (*function)(Args...));

// test functions
void do_nothing(){
    // do nothing
}

double test(){
    return 42.0;
}

double inc(double a){
    return a + 1;
}

const std::vector<STD_LIB_FUNCTION_INFO> STD_LIB_FUNCTIONS_DEFINITIONS = {
    // test functions
    {"do_nothing", make_std_lib_function(do_nothing), "void", {}}, 
    {"test", make_std_lib_function(test), "double", {}},
    {"inc", make_std_lib_function(inc), "double", {"double"}},

    // string functions
    {"string_concat", make_std_lib_function(string_concat), "std::string", {"std::string", "std::string"}},
    {"string_substr", make_std_lib_function(string_substr), "std::string", {"std::string", "int", "int"}},
    {"string_len", make_std_lib_function(string_len), "int", {"std::string"}},
    {"char_at", make_std_lib_function(char_at), "std::string", {"std::string", "int"}},
    {"replace_char", make_std_lib_function(replace_char), "std::string", {"std::string", "int", "std::string"}},
    {"print_colored_text", make_std_lib_function(print_colored_text), "void", {"std::string", "std::string"}},
    {"string_to_vector", make_std_lib_function(string_to_vector), "std::vector<Value>", {"std::string"}},
    {"string_split", make_std_lib_function(string_split), "std::vector<Value>", {"std::string", "std::string"}},
    
    // vector functions
    {"vector_create", make_std_lib_function(vector_create), "std::vector<Value>", {"int", "Value"}},
    {"vector_len", make_std_lib_function(vector_len), "int", {"std::vector<Value>"}},
    {"vector_push", make_std_lib_function(vector_push), "std::vector<Value>", {"std::vector<Value>", "Value"}},
    {"vector_pop", make_std_lib_function(vector_pop), "std::vector<Value>", {"std::vector<Value>"}},
    {"vector_insert", make_std_lib_function(vector_insert), "std::vector<Value>", {"std::vector<Value>", "int", "Value"}},
    {"vector_remove", make_std_lib_function(vector_remove), "std::vector<Value>", {"std::vector<Value>", "int"}},
    {"vector_clear", make_std_lib_function(vector_clear), "std::vector<Value>", {"std::vector<Value>"}},
    {"vector_get", make_std_lib_function(vector_get), "Value", {"std::vector<Value>", "int"}},
    {"vector_set", make_std_lib_function(vector_set), "std::vector<Value>", {"std::vector<Value>", "int", "Value"}},
    {"vector_slice", make_std_lib_function(vector_slice), "std::vector<Value>", {"std::vector<Value>", "int", "int"}},
    {"vector_reverse", make_std_lib_function(vector_reverse), "std::vector<Value>", {"std::vector<Value>"}},
    {"vector_concat", make_std_lib_function(vector_concat), "std::vector<Value>", {"std::vector<Value>", "std::vector<Value>"}},
    
    // file functions
    {"file_write", make_std_lib_function(file_write), "void", {"std::string", "std::string"}},
    {"file_read", make_std_lib_function(file_read), "std::string", {"std::string"}},
    {"run_python_file", make_std_lib_function(run_python_file), "void", {"std::string"}},

    // graphics functions
    {"init_graphics", make_std_lib_function(init_graphics), "int", {"std::string", "int", "int"}},
    {"close_graphics", make_std_lib_function(close_graphics), "void", {}},
    {"clear_screen", make_std_lib_function(clear_screen), "void", {}},
    {"update_screen", make_std_lib_function(update_screen), "void", {}},
    {"draw_rect", make_std_lib_function(draw_rect), "void", {"int", "int", "int", "int"}},

    // random functions
    {"random_number", make_std_lib_function(random_number), "int", {"int", "int"}},
    
};  

void print_std_lib_function(const STD_LIB_FUNCTION_INFO &func){
    std::cout << "Function: " << func.name << std::endl;
    std::cout << "Return Type: " << func.return_type << std::endl;
    std::cout << "Arguments: ";
    for(int i = 0; i < (int)func.arg_types.size(); i++){
        std::cout << func.arg_types[i];
        if(i != (int)func.arg_types.size() - 1){
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

//Takes in a LII Value and a string representing the C++ type
//Return a bool indicating if the LII type can be mapped to the C++ type
bool LII_type_matches_cpp_type(Value value, std::string type){
    switch(value.type){
        case NUMBER:
            if(type == "double" || type == "int"){
                return true;
            }
            break;
        case BOOL:
            if(type == "bool"){
                return true;
            }
            break;
        case STRING:
            if(type == "std::string"){
                return true;
            }
            break;
        case VECTOR:
            if(type == "std::vector<Value>"){
                return true;
            }
            break;
        case FUNCTION:
            return false;
            break;
        default:
            break;
    }

    if(type == "Value"){ 
        return true; // Value can be used as a generic type
    }

    return false;
}

bool any_type_check(std::any value, std::string type){
    if(type == "double"){
        return value.type() == typeid(double);
    }else if(type == "int"){
        return value.type() == typeid(int);
    }else if(type == "bool"){
        return value.type() == typeid(bool);
    }else if(type == "std::string"){
        return value.type() == typeid(std::string);
    }else if(type == "std::vector<Value>"){
        return value.type() == typeid(std::vector<Value>);
    }
    else if(type == "Value"){
        return true; // Value can be used as a generic type
    }
    else{
        std_lib_error("any_type_check", "invalid type [" + type + "]");
        return false;
    }
}

std::any cast_LII_type_to_cpp_type(Value value, std::string type){
    if(type == "double"){
        return VALUE_AS_NUMBER(value);
    }else if(type == "int"){
        return (int)VALUE_AS_NUMBER(value);
    }else if(type == "bool"){
        return VALUE_AS_BOOL(value);
    }else if(type == "std::string"){
        return VALUE_AS_STRING(value);
    }else if(type == "std::vector<Value>"){
        return VALUE_AS_VECTOR(value);
    }else if(type == "Value"){
        return std::any(value);
    }
    else{
        std_lib_error("cast_LII_type_to_cpp_type", "invalid type [" + type + "]");
        return std::any();
    }
}

bool is_correct_number_of_parameters(std::string std_lib_name, int num_params){
    for(int i = 0; i < (int)STD_LIB_FUNCTIONS_DEFINITIONS.size(); i++){
        if(STD_LIB_FUNCTIONS_DEFINITIONS[i].name == std_lib_name){
            if((int)STD_LIB_FUNCTIONS_DEFINITIONS[i].arg_types.size() != num_params){
                return false;
            }
            return true;
        }
    }

    std_lib_error("is_correct_number_of_parameters", "invalid function name [" + std_lib_name + "]");
    return false; // Should never reach here(std_lib error exits), but to avoid warnings
}

// Helper to cast std::any to the correct type and call the function
template<typename Function, typename Tuple, std::size_t... I>
decltype(auto) callWithCastedArgs(Function func, const std::vector<std::any>& args, Tuple&& tuple, std::index_sequence<I...>) {
    return func(std::any_cast<std::tuple_element_t<I, std::decay_t<Tuple>>>(args[I])...);
}

// Main function to convert std::vector<std::any> to a tuple of args and call the function
template<typename Return, typename... Args>
STD_LIB_FUNCTION make_std_lib_function(Return (*function)(Args...)) {
    return [function](std::vector<std::any> args, std::vector<std::string> arg_types) -> std::any {
        if(args.size() != sizeof...(Args)) {
            std_lib_error("make_std_lib_function", "invalid number of arguments");
            return std::any();
        }

        // Assuming args are in the correct order and type
        try {
            if constexpr(!std::is_same_v<Return, void>) {
                auto result = callWithCastedArgs(function, args, std::tuple<Args...>{}, std::index_sequence_for<Args...>{});
                return std::any(result);
            }
            else {
                callWithCastedArgs(function, args, std::tuple<Args...>{}, std::index_sequence_for<Args...>{});
                return std::any();
            }
        } catch(const std::bad_any_cast& e) {
            std_lib_error("make_std_lib_function", "bad any_cast");
        }

        return std::any();
    };
}

#endif // STD_LIB_HPP