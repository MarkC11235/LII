#include "variables.hpp"

std::vector<std::string> Variable::types = {
    "any",
    "number",
    "string",
    "bool",
    "null",
    "vector",
    "map",
    "func",
};

// Constructor
Variable::Variable(std::string name, Declaration_Type declaration_type, std::string type, Value value) 
    : name(name), declaration_type(declaration_type), type(type), value(value) {}

// Getters
std::string Variable::get_name() {
    return name; // Return the name of the variable
}

Declaration_Type Variable::get_declaration_type() {
    return declaration_type; // Return the type of declaration (let, const, global)
}

std::string Variable::get_declaration_type_as_string() const{
    // Convert the declaration type enum to a string
    switch (declaration_type) {
        case DECLARATION_LET: return "let";
        case DECLARATION_CONST: return "const";
        case DECLARATION_GLOBAL: return "global";
        default: return "unknown"; // Return "unknown" if the type is not found
    }
}

std::string Variable::get_type() {
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

bool Variable::is_let() {
    return declaration_type == DECLARATION_LET; // Check if the variable is a let variable
}
bool Variable::is_const() {
    return declaration_type == DECLARATION_CONST; // Check if the variable is a const variable
}
bool Variable::is_global() {
    return declaration_type == DECLARATION_GLOBAL; // Check if the variable is a global variable
}

bool Variable::equal_types(std::string type1, std::string type2) {
    return type1 == type2; // Compare the two types for equality
}

bool Variable::equal_declaration_types(Declaration_Type decl_type1, Declaration_Type decl_type2) {
    return decl_type1 == decl_type2; // Placeholder implementation
}

std::string Variable::declaration_type_to_string(Declaration_Type decl_type) {
    switch (decl_type) {
        case DECLARATION_LET: return "let";
        case DECLARATION_CONST: return "const";
        case DECLARATION_GLOBAL: return "global";
        default: return "unknown";
    }
}

Declaration_Type Variable::string_to_declaration_type(std::string decl_type_str) {
    if (decl_type_str == "let") {
        return DECLARATION_LET;
    } else if (decl_type_str == "const") {
        return DECLARATION_CONST;
    } else if (decl_type_str == "global") {
        return DECLARATION_GLOBAL;
    }
    throw std::invalid_argument("Invalid declaration type string");
}

bool Variable::is_valid_type(std::string type){
    for(const auto& valid_type : types) {
        if (valid_type == type) {
            return true; // Return true if the type is valid
        }
    }
    return false; // Return false if the type is not found in the valid types
}

void Variable::add_variable_type(std::string type){
    if (!is_valid_type(type)) {
        types.push_back(type); // Add the new type to the list of valid types
    }
    // If the type already exists, throw an exception or handle it as needed
    else {
        std::cerr << "Type already exists: " << type << "\n";
        std::cout << "TODO: Handle duplicate type addition gracefully.\n";
        exit(1); // Exit the program with an error code
    }
}

int Variable::type_index(std::string type) {
    for (size_t i = 0; i < types.size(); ++i) {
        if (types[i] == type) {
            return static_cast<int>(i); // Return the index of the type
        }
    }
    return -1; // Return -1 if the type is not found
}

std::string Variable::type_to_string(int type_index) {
    if (type_index < 0 || type_index >= static_cast<int>(types.size())) {
        std::cerr << "Type index out of bounds: " << type_index << "\n";
        std::cout << "TODO: Handle out of bounds type index gracefully.\n";
        exit(1); // Exit the program with an error code
    }
    return types[type_index]; // Return the type as a string based on the index
}
