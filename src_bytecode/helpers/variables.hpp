#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include <iostream>
#include <string>
#include <map>
#include "Value.hpp"


enum Declaration_Type {
    DECLARATION_LET,   // let variable
    DECLARATION_CONST, // const variable
    DECLARATION_GLOBAL // global variable
};

class Variable {
    private:
    static std::vector<std::string> types;
    std::string name; // Name of the variable
    Declaration_Type declaration_type; // Type of declaration (let, const, global)
    std::string type;
    Value value; // The value of the variable, can be of different types (int, float, string, etc.)


public:
    // Default constructor
    Variable() : name(""), declaration_type(DECLARATION_LET), type("any"), value(Value()) {}
    Variable(std::string name, Declaration_Type declaration_type, std::string type, Value value);
    
    // Getters
    std::string get_name();
    Declaration_Type get_declaration_type();
    std::string get_declaration_type_as_string() const;
    std::string get_type();
    Value get_value() const;

    // Setters
    void set_value(Value new_value);
    
    // Other methods
    void print(); // For debugging purposes
    bool is_let();
    bool is_const();
    bool is_global();
    
    static bool equal_types(std::string type1, std::string type2);

    static bool equal_declaration_types(Declaration_Type decl_type1, Declaration_Type decl_type2);

    static std::string declaration_type_to_string(Declaration_Type decl_type);

    static Declaration_Type string_to_declaration_type(std::string decl_type_str);

    // variable type map functions 
    static bool is_valid_type(std::string type);
    static void add_variable_type(std::string type);
    static int type_index(std::string type);
    static std::string type_to_string(int type_index);

};

#endif // VARIABLES_HPP