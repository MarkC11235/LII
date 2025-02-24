#ifndef VALUE_HPP
#define VALUE_HPP

#include <variant>
#include <vector>
#include <string>
#include <iostream>
#include <charconv>
#include <array>
#include "Function.hpp"

void display_bytecode(function* func);

/*
All the types that the language supports
*/
enum Value_Type{
    NUMBER,
    BOOL,
    STRING,
    VECTOR,
    FUNCTION,
    NULL_VALUE,
    MAP,
};

// forward declare Value for the typedef (recursive for map and vector)
struct Value;

typedef std::variant<
                    double, // NUMBER
                    bool, // BOOL
                    std::string, // STRING
                    std::vector<Value>, // VECTOR
                    function*, // FUNCTION
                    std::nullptr_t, // NULL_VALUE
                    std::map<std::string, Value> // MAP
                    > Value_Content;

/*
Value struct that holds the type and the data
*/
struct Value{
    Value_Type type;
    Value_Content data;

    Value(){}

    Value(Value_Type t, Value_Content d){
        type = t;
        data = d;
    }

    Value(const Value& other){
        type = other.type;
        data = other.data; 
    }
};

/*
Pass in a Value and get the Value_Type of the Value
*/
Value_Type get_value_type(Value value){
    return value.type;
}

/*
Pass in the string representation of the type and get the Value_Type
Returns NULL_VALUE if the type is not found 
*/
Value_Type get_value_type_from_string(std::string type){
    if(type == "number"){
        return Value_Type::NUMBER;
    } else if(type == "bool"){
        return Value_Type::BOOL;
    } else if(type == "string"){
        return Value_Type::STRING;
    } else if(type == "vector"){
        return Value_Type::VECTOR;
    } else if(type == "function"){
        return Value_Type::FUNCTION;
    } else if(type == "null"){
        return Value_Type::NULL_VALUE;
    } else if(type == "map"){
        return Value_Type::MAP;
    } else {
        return Value_Type::NULL_VALUE;
    }
}

/*
Pass in a Value and get the string representation of the type
Returns "unknown" if the type is not found
*/
std::string get_value_type_string(Value value){
    switch(value.type){
        case NUMBER:
            return "number";
        case BOOL:
            return "bool";
        case STRING:
            return "string";
        case VECTOR:
            return "vector";
        case FUNCTION:
            return "function";
        case NULL_VALUE:
            return "null";
        case MAP:
            return "map";
        default:
            return "unknown"; // Should never reach here, but to avoid warnings
    }
}

bool VALUE_AS_BOOL(Value value);
double VALUE_AS_NUMBER(Value value);
std::string VALUE_AS_STRING(Value value);
std::vector<Value> VALUE_AS_VECTOR(Value value);
function* VALUE_AS_FUNCTION(Value value);
std::map<std::string, Value> VALUE_AS_MAP(Value value);

/*
Pass in a Value and coerse the value to a bool
*/
bool VALUE_AS_BOOL(Value value){
    switch(value.type){
        case BOOL:
            return std::get<bool>(value.data);
        case NUMBER:
            return std::get<double>(value.data) != 0;
        case STRING:
            return std::get<std::string>(value.data) != "";
        default:
            return false; // Should never reach here, but to avoid warnings
    }
}

/*
Pass in a Value and coerse the value to a double
*/
double VALUE_AS_NUMBER(Value value){
    switch(value.type){
        case NUMBER:
            return std::get<double>(value.data);
        case BOOL:
            return std::get<bool>(value.data);
        case STRING:
            return std::stod(std::get<std::string>(value.data));
        default:
            return 0; // Should never reach here, but to avoid warnings
    }
}

/*
Pass in a Value and coerse the value to a string
*/
std::string VALUE_AS_STRING(Value value){
    // std::cout << "VALUE AS STRING | Value_Type: " << get_value_type_string(value) << std::endl;
    switch(value.type){
        case NUMBER: // TODO: Improve this to be faster, this is just a quick fix to remove trailing zeros
                     // This is because the tests expect the output to not have trailing zeros
                     // Could use some fancy math things to remove trailing zeros
        {
            // std::string str = std::to_string(std::get<double>(value.data));
            // if(str.find('.') != std::string::npos){
            //     str.erase(str.find_last_not_of('0') + 1, std::string::npos);
            //     if(str[str.size() - 1] == '.'){
            //         str.pop_back();
            //     }
            // }
            // return str;

            // This method can be used to remove trailing zeros, get more precision, and remove the decimal point if the number is an integer
            std::array<char, 32> buffer;  // Buffer size enough for full precision

            auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), 
                                            std::get<double>(value.data), std::chars_format::general);
            std::string str(buffer.data(), ptr);  // Store result in std::string
            return str;
        }
        case BOOL:
            return std::get<bool>(value.data) ? "true" : "false";
        case STRING:
        {
            std::string str = std::get<std::string>(value.data);
            return str;
        }
        case VECTOR:
        {
            std::string str = "[";

            std::vector<Value> vec = std::get<std::vector<Value>>(value.data);
            for(int i = 0; i < (int)vec.size(); i++){
                str += VALUE_AS_STRING(vec[i]);
                if(i != (int)vec.size() - 1){
                    str += ", ";
                }
            }

            str += "]";
            return str;
        }
        case FUNCTION:
        {
            function* func = std::get<function*>(value.data);
            std::string args = "(";
            for(int i = 0; i < (int)func->arguments.size(); i++){
                args += func->arguments[i];
                if(i != (int)func->arguments.size() - 1){
                    args += ", ";
                }
            }
            args += ")";

            std::string bc = "";
            for(int i = 0; i < func->count; i++){
                bc += std::to_string(func->code[i]) + ", ";
            }
            return "function " + func->name + args + " {" + bc + "}";
        }
        case NULL_VALUE:
            return "null";
        case MAP:
        {
            // std::string str = "{";

            // std::map<std::string, Value> map = std::get<std::map<std::string, Value>>(value.data);
            // // prints the keys in alphabetical order
            // // ig thats how c++ stores the keys internally
            // for(auto it = map.begin(); it != map.end(); it++){
            //     str += it->first + " = " + VALUE_AS_STRING(it->second);
            //     if(it != --map.end()){
            //         str += ", ";
            //     }
            // }

            // str += "}";
            // return str;

            // Prints as a json object
            // This code is from the std_lib/files.hpp file
            // I just copied it here to avoid including the file (circular dependency)
            std::map<std::string, Value> map = std::get<std::map<std::string, Value>>(value.data);
            std::string json = "{";
            for (auto it = map.begin(); it != map.end(); it++)
            {
                json += "\"" + it->first + "\":";
                // json += value_to_json(it->second);

                Value value = it->second;
                if (get_value_type(value) == Value_Type::STRING || get_value_type(value) == Value_Type::FUNCTION)
                {
                    json += "\"" + VALUE_AS_STRING(value) + "\"";
                }
                else if (get_value_type(value) == Value_Type::VECTOR)
                {
                    json += "[";
                    std::vector<Value> vec = VALUE_AS_VECTOR(value);
                    for (int i = 0; i < (int)vec.size(); i++)
                    {
                        if(get_value_type(vec[i]) == Value_Type::STRING || get_value_type(vec[i]) == Value_Type::FUNCTION){
                            json += "\"" + VALUE_AS_STRING(vec[i]) + "\"";
                        } else {
                            json += VALUE_AS_STRING(vec[i]);
                        }
                        if (i != (int)vec.size() - 1)
                        {
                            json += ",";
                        }
                    }
                    json += "]";

                }
                else
                {
                    json += VALUE_AS_STRING(value);
                }

                if (it != --map.end())
                {
                    json += ",";
                }
            }
            json += "}";
            return json;
        }
        default:
            return "UNKNOWN"; // Should never reach here, but to avoid warnings
    }
}

/*
Pass in a Value and coerse the value to a vector
*/
std::vector<Value> VALUE_AS_VECTOR(Value value){
    switch(value.type){
        case VECTOR:
            return std::get<std::vector<Value>>(value.data);
        default:
            return {}; // Should never reach here, but to avoid warnings
    }
}

/*
Pass in a Value and coerse the value to a function
*/
function* VALUE_AS_FUNCTION(Value value){
    if(value.type == Value_Type::FUNCTION){
        function* func = std::get<function*>(value.data);
        return func;
    }

    std::cout << "ERROR: casting non function to function" << std::endl;
    exit(1);
    return nullptr; // will never reach here
}

/*
Pass in a Value and coerse the value to a map
*/
std::map<std::string, Value> VALUE_AS_MAP(Value value){
    switch(value.type){
        case MAP:
            return std::get<std::map<std::string, Value>>(value.data);
        default:
            return {}; // Should never reach here, but to avoid warnings
    }
}

/*
Pass in a Value and print the string representation of the value
*/
void print_value(Value value, bool verbose = false){
    // TODO: Add support for printing functions inside JITed functions
    if(verbose) {
        std::cout << "Type: " << get_value_type_string(value) << " | ";
    }
    if(value.type == Value_Type::FUNCTION){
        std::cout << "Function: \n";
        display_bytecode(VALUE_AS_FUNCTION(value));
        return;
    }
    std::cout << VALUE_AS_STRING(value);
}

#endif // VALUE_HPP