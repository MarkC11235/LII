#ifndef VALUE_HPP
#define VALUE_HPP

#include <variant>
#include <vector>
#include <string>
#include <iostream>

enum Value_Type{
    NUMBER,
    BOOL,
    STRING,
    VECTOR,
    //STRUCT, Not implemented
};

struct Value{
    Value_Type type;
    std::variant<
                double, // NUMBER
                bool, // BOOL
                std::string, // STRING
                std::vector<Value> // VECTOR
                > data;
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
            return std::get<std::string>(value.data);
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

void print_value(Value value){
    std::cout << VALUE_AS_STRING(value);
}

#endif // VALUE_HPP