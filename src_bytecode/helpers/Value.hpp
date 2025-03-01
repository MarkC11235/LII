#ifndef VALUE_HPP
#define VALUE_HPP

#include <variant>
#include <vector>
#include <string>
#include <iostream>
#include <charconv>
#include <array>
#include <map>
#include "Function.hpp"

void display_bytecode(function* func);

enum Value_Type {
    NUMBER,
    BOOL,
    STRING,
    VECTOR,
    FUNCTION,
    NULL_VALUE,
    MAP,
};

// forward declare Value for the typedef (recursive for map and vector)
class Value;

typedef std::variant<
    double,              // NUMBER
    bool,                // BOOL
    std::string,         // STRING
    std::vector<Value>,  // VECTOR
    function*,           // FUNCTION
    std::nullptr_t,      // NULL_VALUE
    std::map<std::string, Value> // MAP
> Value_Content;

class Value {
public:
    Value_Type type;
    Value_Content data;

    Value();
    Value(Value_Type t, Value_Content d);
    Value(const Value& other);

    static bool equals(Value a, Value b);
};

Value_Type get_value_type(Value value);
Value_Type get_value_type_from_string(std::string type);
std::string get_value_type_string(Value value);

bool VALUE_AS_BOOL(Value value);
double VALUE_AS_NUMBER(Value value);
std::string VALUE_AS_STRING(Value value);
std::vector<Value> VALUE_AS_VECTOR(Value value);
function* VALUE_AS_FUNCTION(Value value);
std::map<std::string, Value> VALUE_AS_MAP(Value value);
void print_value(Value value, bool verbose = false);

#endif // VALUE_HPP