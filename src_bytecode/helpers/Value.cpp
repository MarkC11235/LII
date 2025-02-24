#include "Value.hpp"

Value::Value() {}

Value::Value(Value_Type t, Value_Content d) {
    type = t;
    data = d;
}

Value::Value(const Value& other) {
    type = other.type;
    data = other.data;
}

Value_Type get_value_type(Value value) {
    return value.type;
}

Value_Type get_value_type_from_string(std::string type) {
    if(type == "number") {
        return Value_Type::NUMBER;
    } else if(type == "bool") {
        return Value_Type::BOOL;
    } else if(type == "string") {
        return Value_Type::STRING;
    } else if(type == "vector") {
        return Value_Type::VECTOR;
    } else if(type == "function") {
        return Value_Type::FUNCTION;
    } else if(type == "null") {
        return Value_Type::NULL_VALUE;
    } else if(type == "map") {
        return Value_Type::MAP;
    }
    return Value_Type::NULL_VALUE;
}

std::string get_value_type_string(Value value) {
    switch(value.type) {
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
            return "unknown";
    }
}

bool VALUE_AS_BOOL(Value value) {
    switch(value.type) {
        case BOOL:
            return std::get<bool>(value.data);
        case NUMBER:
            return std::get<double>(value.data) != 0;
        case STRING:
            return std::get<std::string>(value.data) != "";
        default:
            return false;
    }
}

double VALUE_AS_NUMBER(Value value) {
    switch(value.type) {
        case NUMBER:
            return std::get<double>(value.data);
        case BOOL:
            return std::get<bool>(value.data);
        case STRING:
            return std::stod(std::get<std::string>(value.data));
        default:
            return 0;
    }
}

std::string VALUE_AS_STRING(Value value) {
    switch(value.type) {
        case NUMBER: {
            std::array<char, 32> buffer;
            auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), 
                                         std::get<double>(value.data), 
                                         std::chars_format::general);
            return std::string(buffer.data(), ptr);
        }
        case BOOL:
            return std::get<bool>(value.data) ? "true" : "false";
        case STRING:
            return std::get<std::string>(value.data);
        case VECTOR: {
            std::string str = "[";
            std::vector<Value> vec = std::get<std::vector<Value>>(value.data);
            for(int i = 0; i < (int)vec.size(); i++) {
                str += VALUE_AS_STRING(vec[i]);
                if(i != (int)vec.size() - 1) {
                    str += ", ";
                }
            }
            str += "]";
            return str;
        }
        case FUNCTION: {
            function* func = std::get<function*>(value.data);
            std::string args = "(";
            for(int i = 0; i < (int)func->arguments.size(); i++) {
                args += func->arguments[i];
                if(i != (int)func->arguments.size() - 1) {
                    args += ", ";
                }
            }
            args += ")";
            std::string bc = "";
            for(int i = 0; i < func->count; i++) {
                bc += std::to_string(func->code[i]) + ", ";
            }
            return "function " + func->name + args + " {" + bc + "}";
        }
        case NULL_VALUE:
            return "null";
        case MAP: {
            std::map<std::string, Value> map = std::get<std::map<std::string, Value>>(value.data);
            std::string json = "{";
            for (auto it = map.begin(); it != map.end(); it++) {
                json += "\"" + it->first + "\":";
                Value val = it->second;
                if (get_value_type(val) == Value_Type::STRING || 
                    get_value_type(val) == Value_Type::FUNCTION) {
                    json += "\"" + VALUE_AS_STRING(val) + "\"";
                }
                else if (get_value_type(val) == Value_Type::VECTOR) {
                    json += "[";
                    std::vector<Value> vec = VALUE_AS_VECTOR(val);
                    for (int i = 0; i < (int)vec.size(); i++) {
                        if(get_value_type(vec[i]) == Value_Type::STRING || 
                           get_value_type(vec[i]) == Value_Type::FUNCTION) {
                            json += "\"" + VALUE_AS_STRING(vec[i]) + "\"";
                        } else {
                            json += VALUE_AS_STRING(vec[i]);
                        }
                        if (i != (int)vec.size() - 1) {
                            json += ",";
                        }
                    }
                    json += "]";
                }
                else {
                    json += VALUE_AS_STRING(val);
                }
                if (it != --map.end()) {
                    json += ",";
                }
            }
            json += "}";
            return json;
        }
        default:
            return "UNKNOWN";
    }
}

std::vector<Value> VALUE_AS_VECTOR(Value value) {
    switch(value.type) {
        case VECTOR:
            return std::get<std::vector<Value>>(value.data);
        default:
            return {};
    }
}

function* VALUE_AS_FUNCTION(Value value) {
    if(value.type == Value_Type::FUNCTION) {
        return std::get<function*>(value.data);
    }
    std::cout << "ERROR: casting non function to function" << std::endl;
    exit(1);
}

std::map<std::string, Value> VALUE_AS_MAP(Value value) {
    switch(value.type) {
        case MAP:
            return std::get<std::map<std::string, Value>>(value.data);
        default:
            return {};
    }
}

void print_value(Value value, bool verbose) {
    if(verbose) {
        std::cout << "Type: " << get_value_type_string(value) << " | ";
    }
    if(value.type == Value_Type::FUNCTION) {
        std::cout << "Function: \n";
        display_bytecode(VALUE_AS_FUNCTION(value));
        return;
    }
    std::cout << VALUE_AS_STRING(value);
}