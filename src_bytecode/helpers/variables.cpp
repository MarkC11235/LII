#include "variables.hpp"

std::map<int, std::string> variable_type_map = {
    {0, "any"},
    {1, "number"},
    {2, "string"},
    {3, "bool"},
    {4, "null"},
    {5, "vector"},
    {6, "map"},
    {7, "func"},
};

// Constructor
Variable::Variable(std::string name, Declaration_Type declaration_type, int type, Value value) 
    : name(name), declaration_type(declaration_type), type(type), value(value) {}

// Getters
std::string Variable::get_name() {
    return name; // Return the name of the variable
}

Declaration_Type Variable::get_declaration_type() {
    return declaration_type; // Return the type of declaration (let, const, global)
}

int Variable::get_type() {
    return type; // Return the type of the variable (e.g., number, string, etc.)
}

Value Variable::get_value() const {
    return value; // Return the current value of the variable
}


// Setters
void Variable::set_value(Value new_value) {
    value = new_value; // Update the value of the variable
}

// Other methods
void Variable::print() {
    std::cout << "Variable Name: " << name << "\n";
    std::cout << "Declaration Type: " << /* Convert declaration_type to string if needed */ "\n";
    std::cout << "Variable Type: " << /* Convert type to string if needed */ "\n";
    std::cout << "Value: ";
    print_value(value);
    std::cout << "\n";
}

