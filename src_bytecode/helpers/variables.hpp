#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include <iostream>
#include <string>
#include <map>
#include "Value.hpp"

extern std::map<int, std::string> variable_type_map;

enum Declaration_Type {
    DECLARATION_LET,   // let variable
    DECLARATION_CONST, // const variable
    DECLARATION_GLOBAL // global variable
};

class Variable {
private:
    std::string name; // Name of the variable
    Declaration_Type declaration_type; // Type of declaration (let, const, global)
    int type;
    Value value; // The value of the variable, can be of different types (int, float, string, etc.)


public:
    // Default constructor
    Variable() : name(""), declaration_type(DECLARATION_LET), type(0), value(Value()) {}
    Variable(std::string name, Declaration_Type declaration_type, int type, Value value);
    
    // Getters
    std::string get_name();
    Declaration_Type get_declaration_type();
    std::string get_declaration_type_as_string() {
        // Convert the declaration type enum to a string
        switch (declaration_type) {
            case DECLARATION_LET: return "let";
            case DECLARATION_CONST: return "const";
            case DECLARATION_GLOBAL: return "global";
            default: return "unknown"; // Return "unknown" if the type is not found
        }
    }
    int get_type();
    std::string get_type_as_string() {
        // Convert the type integer to a string using the map
        if (variable_type_map.find(type) != variable_type_map.end()) {
            return variable_type_map[type];
        }
        return "unknown"; // Return "unknown" if the type is not found
    }
    Value get_value() const;

    // Setters
    void set_value(Value new_value);
    
    // Other methods
    void print(); // For debugging purposes
    bool is_let() {
        return declaration_type == DECLARATION_LET; // Check if the variable is a let variable
    }
    bool is_const() {
        return declaration_type == DECLARATION_CONST; // Check if the variable is a const variable
    }
    bool is_global() {
        return declaration_type == DECLARATION_GLOBAL; // Check if the variable is a global variable
    }
    
    static bool equal_types(int type1, int type2) {
        return type1 == type2; // Placeholder implementation
    }

    static bool equal_declaration_types(Declaration_Type decl_type1, Declaration_Type decl_type2) {
        return decl_type1 == decl_type2; // Placeholder implementation
    }

    static std::string declaration_type_to_string(Declaration_Type decl_type) {
        switch (decl_type) {
            case DECLARATION_LET: return "let";
            case DECLARATION_CONST: return "const";
            case DECLARATION_GLOBAL: return "global";
            default: return "unknown";
        }
    }

    static Declaration_Type string_to_declaration_type(std::string decl_type_str) {
        if (decl_type_str == "let") {
            return DECLARATION_LET;
        } else if (decl_type_str == "const") {
            return DECLARATION_CONST;
        } else if (decl_type_str == "global") {
            return DECLARATION_GLOBAL;
        }
        throw std::invalid_argument("Invalid declaration type string");
    }

    static std::string type_to_string(int type) {
        // Convert the variable type to a string using the map
        if (variable_type_map.find(type) != variable_type_map.end()) {
            return variable_type_map[type];
        }
        return "unknown"; // Return "unknown" if the type is not found
    }

    static int string_to_type(std::string type_str) {
        // Convert a string to the corresponding variable type integer
        for (const auto& pair : variable_type_map) {
            if (pair.second == type_str) {
                return pair.first; // Return the corresponding integer type
            }
        }
        return -1; // Return -1 if the string does not match any type
    }

};

#endif // VARIABLES_HPP