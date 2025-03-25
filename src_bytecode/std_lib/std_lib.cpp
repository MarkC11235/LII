#include "std_lib.hpp"

// test functions --------------------------------------------------------------------------------------------
int do_nothing(){
    return 0;
}

double test(){
    return 42.0;
}

double inc(double a){
    return a + 1;
}
// ------------------------------------------------------------------------------------------------------------

/*
List of standard library functions
C++ functions need to be define here to be used in CastleLang
*/
const std::vector<STD_LIB_FUNCTION_INFO> STD_LIB_FUNCTIONS_DEFINITIONS = {
    // test functions
    {"do_nothing", make_std_lib_function(do_nothing), "int", {}}, 
    {"test", make_std_lib_function(test), "double", {}},
    {"inc", make_std_lib_function(inc), "double", {"double"}},

    // string functions
    {"string_join", make_std_lib_function(string_join), "std::string", {"std::string", "std::string"}},
    {"string_substring", make_std_lib_function(string_substring), "std::string", {"std::string", "int", "int"}},
    {"string_length", make_std_lib_function(string_length), "int", {"std::string"}},
    {"char_at", make_std_lib_function(char_at), "std::string", {"std::string", "int"}},
    {"replace_char", make_std_lib_function(replace_char), "std::string", {"std::string", "int", "std::string"}},
    {"print_colored_text", make_std_lib_function(print_colored_text), "int", {"std::string", "std::string"}},
    {"string_to_vector", make_std_lib_function(string_to_vector), "std::vector<Value>", {"std::string"}},
    {"string_split", make_std_lib_function(string_split), "std::vector<Value>", {"std::string", "std::string"}},
    
    // vector functions
    {"vector_create", make_std_lib_function(vector_create), "std::vector<Value>", {"int", "Value"}},
    {"vector_length", make_std_lib_function(vector_length), "int", {"std::vector<Value>"}},
    {"vector_push", make_std_lib_function(vector_push), "std::vector<Value>", {"std::vector<Value>", "Value"}},
    {"vector_pop", make_std_lib_function(vector_pop), "std::vector<Value>", {"std::vector<Value>"}},
    {"vector_insert", make_std_lib_function(vector_insert), "std::vector<Value>", {"std::vector<Value>", "int", "Value"}},
    {"vector_remove", make_std_lib_function(vector_remove), "std::vector<Value>", {"std::vector<Value>", "int"}},
    {"vector_clear", make_std_lib_function(vector_clear), "std::vector<Value>", {"std::vector<Value>"}},
    {"vector_get", make_std_lib_function(vector_get), "Value", {"std::vector<Value>", "int"}},
    {"vector_set", make_std_lib_function(vector_set), "std::vector<Value>", {"std::vector<Value>", "int", "Value"}},
    {"vector_slice", make_std_lib_function(vector_slice), "std::vector<Value>", {"std::vector<Value>", "int", "int"}},
    {"vector_reverse", make_std_lib_function(vector_reverse), "std::vector<Value>", {"std::vector<Value>"}},
    {"vector_join", make_std_lib_function(vector_join), "std::vector<Value>", {"std::vector<Value>", "std::vector<Value>"}},
    
    // map functions
    {"map_size", make_std_lib_function(map_size), "int", {"std::map<std::string, Value>"}},
    {"map_join", make_std_lib_function(map_join), "std::map<std::string, Value>", {"std::map<std::string, Value>", "std::map<std::string, Value>"}},
    
    // file functions
    {"file_write", make_std_lib_function(file_write), "int", {"std::string", "std::string"}},
    {"file_write_lines", make_std_lib_function(file_write_lines), "int", {"std::string", "std::vector<Value>"}},
    {"file_read", make_std_lib_function(file_read), "std::string", {"std::string"}},
    {"file_read_lines", make_std_lib_function(file_read_lines), "std::vector<Value>", {"std::string"}},
    {"stdin_read", make_std_lib_function(stdin_read), "std::string", {}},
    {"csv_write", make_std_lib_function(csv_write), "int", {"std::string", "std::vector<Value>"}},
    {"csv_read", make_std_lib_function(csv_read), "std::vector<Value>", {"std::string"}}, // Returns a vector of vectors of strings
    {"run_python_file", make_std_lib_function(run_python_file), "int", {"std::string"}},
    {"map_to_json", make_std_lib_function(map_to_json), "std::string", {"std::map<std::string, Value>"}},
    {"json_to_map", make_std_lib_function(json_to_map), "std::map<std::string, Value>", {"std::string"}},

    // graphics functions
    {"get_events", make_std_lib_function(get_events), "std::vector<Value>", {}},
    {"init_graphics", make_std_lib_function(init_graphics), "int", {"std::string", "int", "int"}},
    {"close_graphics", make_std_lib_function(close_graphics), "int", {}},
    {"clear_screen", make_std_lib_function(clear_screen), "int", {}},
    {"update_screen", make_std_lib_function(update_screen), "int", {}},
    {"change_color", make_std_lib_function(change_color), "int", {"int", "int", "int"}},
    {"draw_rect", make_std_lib_function(draw_rect), "int", {"int", "int", "int", "int"}},
    {"draw_line", make_std_lib_function(draw_line), "int", {"int", "int", "int", "int"}},
    {"draw_circle", make_std_lib_function(draw_circle), "int", {"int", "int", "int"}},
    {"draw_lines", make_std_lib_function(draw_lines), "int", {"std::vector<Value>"}},
    {"draw_text", make_std_lib_function(draw_text), "int", {"std::string", "int", "int", "int"}},
    {"graph", make_std_lib_function(graph), "int", {"std::vector<Value>", "std::vector<Value>", "double", "double", "double", "double", "std::string", "std::string", "std::string"}},

    // misc functions
    {"exit_program", make_std_lib_function(exit_program), "int", {"std::string"}},
    {"wait", make_std_lib_function(wait), "int", {"double"}},
    {"system_command", make_std_lib_function(system_command), "int", {"std::string"}},

    // http functions
    {"start_server", make_std_lib_function(start_server), "int", {"int"}},
    {"stop_server", make_std_lib_function(stop_server), "int", {}},
    {"pop_request", make_std_lib_function(pop_request), "std::map<std::string, Value>", {}},
    {"push_response", make_std_lib_function(push_response), "int", {"int", "std::map<std::string, Value>"}},
    {"server_should_close", make_std_lib_function(server_should_close), "bool", {}},

    // http client functions
    {"send_request", make_std_lib_function(send_request), "std::map<std::string, Value>", {"std::string", "int", "std::map<std::string, Value>"}},

    // Matrix operations
    {"matrix_multiply", make_std_lib_function(matrix_multiply), "std::vector<Value>", {"std::vector<Value>", "std::vector<Value>"}},

    // Math 
    {"random_int", make_std_lib_function(random_int), "int", {"int", "int"}},
    {"sqrt", make_std_lib_function(math_sqrt), "double", {"double"}},
    {"pow", make_std_lib_function(math_pow), "double", {"double", "double"}},
};

/*
Prints the name of the function and the type of args and return type 
*/
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

/*
Takes in a LII Value and a string representing the C++ type
Return a bool indicating if the LII type can be mapped to the C++ type
*/
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
        case MAP:
            if(type == "std::map<std::string, Value>"){
                return true;
            }
            break;
        default:
            break;
    }

    if(type == "Value"){ 
        return true; // Value can be used as a generic type
    }

    return false;
}

/*
Takes in a std::any and a string representing the C++ type
Return a bool indicating if the std::any type can be mapped to the C++ type
*/
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
    else if(type == "std::map<std::string, Value>"){
        return value.type() == typeid(std::map<std::string, Value>);
    }
    else if(type == "Value"){
        return true; // Value can be used as a generic type
    }
    else{
        std_lib_error("any_type_check", "invalid type [" + type + "]");
        return false;
    }
}

/*
Takes in a LII Value and a string representing the C++ type
Return a std::any of the LII Value casted to the C++ type
*/
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
    }else if(type == "std::map<std::string, Value>"){
        return VALUE_AS_MAP(value);
    }
    else if(type == "Value"){
        return std::any(value);
    }
    else{
        std_lib_error("cast_LII_type_to_cpp_type", "invalid type [" + type + "]");
        return std::any();
    }
}

/*
Searches for the function in the list of standard library functions
If the function is not found, an error is printed and the program exits
If the function is found it returns true or false depending on if the number of parameters is correct
*/
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

/*
Calls the given std_lib function with the given arguments
Does it by decaing the tuple into its individual elements and casting the arguments with any_cast 
Can do this safely because all the types are checked before calling this function
*/
template<typename Function, typename Tuple, std::size_t... I>
decltype(auto) callWithCastedArgs(Function func, const std::vector<std::any>& args, Tuple&& tuple, std::index_sequence<I...>) {
    return func(std::any_cast<std::tuple_element_t<I, std::decay_t<Tuple>>>(args[I])...);
}

/*
Creates a lambda function that is a wrapper for callWithCastedArgs
All functions in the STD_LIB_FUNCTIONS_DEFINITIONS get passed through this function to get a function pointer
*/
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
            else { // TODO: probably make this an error becasue you shouldn't have void return types in the std lib
                callWithCastedArgs(function, args, std::tuple<Args...>{}, std::index_sequence_for<Args...>{});
                return std::any();
            }
        } catch(const std::bad_any_cast& e) {
            std_lib_error("make_std_lib_function", "bad any_cast");
        }

        return std::any();
    };
}
