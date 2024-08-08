#ifndef VALUE_HPP
#define VALUE_HPP

#include <variant>
#include <vector>
#include <string>
#include <iostream>
#include "Function.hpp"


void display_bytecode(function* func);

enum Value_Type{
    NUMBER,
    BOOL,
    STRING,
    VECTOR,
    FUNCTION,
    NULL_VALUE,
    STRUCT,
};

// forward declare Value for the typedef
struct Value;

typedef std::variant<
                    double, // NUMBER
                    bool, // BOOL
                    std::string, // STRING
                    std::vector<Value>, // VECTOR
                    function*,
                    std::nullptr_t, // NULL_VALUE
                    std::map<std::string, Value> // STRUCT
                    > Value_Content;

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

Value_Type get_value_type(Value value){
    return value.type;
}

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
        case STRUCT:
            return "struct";
        default:
            return "unknown"; // Should never reach here, but to avoid warnings
    }
}

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

std::string VALUE_AS_STRING(Value value){
    // std::cout << "VALUE AS STRING | Value_Type: " << get_value_type_string(value) << std::endl;
    switch(value.type){
        case NUMBER: // TODO: Improve this to be faster, this is just a quick fix to remove trailing zeros
                     // This is because the tests expect the output to not have trailing zeros
                     // Could use some fancy math things to remove trailing zeros
        {
            std::string str = std::to_string(std::get<double>(value.data));
            if(str.find('.') != std::string::npos){
                str.erase(str.find_last_not_of('0') + 1, std::string::npos);
                if(str[str.size() - 1] == '.'){
                    str.pop_back();
                }
            }
            return str;
        }
        case BOOL:
            return std::get<bool>(value.data) ? "true" : "false";
        case STRING:
        {
            //replace all \\n with \n
            std::string str = std::get<std::string>(value.data);
            size_t pos = 0;
            while((pos = str.find("\\n", pos)) != std::string::npos){
                str.replace(pos, 2, "\n");
                pos += 1;
            }

            //replace all \\t with \t
            pos = 0;
            while((pos = str.find("\\t", pos)) != std::string::npos){
                str.replace(pos, 2, "\t");
                pos += 1;
            }

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
            std::string bc = "";
            for(int i = 0; i < func->count; i++){
                bc += std::to_string(func->code[i]) + ", ";
            }
            return "Function: " + bc;
        }
        case NULL_VALUE:
            return "null";
        case STRUCT:
        {
            std::string str = "{";

            std::map<std::string, Value> map = std::get<std::map<std::string, Value>>(value.data);
            // prints the keys in alphabetical order
            // ig thats how c++ stores the keys internally
            for(auto it = map.begin(); it != map.end(); it++){
                str += it->first + " = " + VALUE_AS_STRING(it->second);
                if(it != --map.end()){
                    str += ", ";
                }
            }

            str += "}";
            return str;
        }
        default:
            return "UNKNOWN"; // Should never reach here, but to avoid warnings
    }
}

std::vector<Value> VALUE_AS_VECTOR(Value value){
    switch(value.type){
        case VECTOR:
            return std::get<std::vector<Value>>(value.data);
        default:
            return {}; // Should never reach here, but to avoid warnings
    }
}

function* VALUE_AS_FUNCTION(Value value){
    if(value.type == Value_Type::FUNCTION){
        return std::get<function*>(value.data);
    }

    std::cout << "ERROR: casting non function to function" << std::endl;
    exit(1);
    return nullptr; // will never reach here
}

std::map<std::string, Value> VALUE_AS_STRUCT(Value value){
    switch(value.type){
        case STRUCT:
            return std::get<std::map<std::string, Value>>(value.data);
        default:
            return {}; // Should never reach here, but to avoid warnings
    }
}

void print_value(Value value, bool verbose = false){
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