#ifndef BYTECODE_GENERATOR_HPP
#define BYTECODE_GENERATOR_HPP

#include <cstdint> // int8_t
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <unordered_map>

#include "parser.hpp"
#include "std_lib.hpp"

enum OpCode{
    // Arithmetic
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    //Comparison
    OP_EQ,
    OP_NEQ,
    OP_GT,
    OP_LT,
    OP_GTEQ,
    OP_LTEQ,
    // Variables
    OP_LOAD, // push a value from the values array to the stack, index is the next byte
    OP_STORE_VAR, // store a value from the stack to the variables map
    OP_UPDATE_VAR, // update a value in the variables map
    OP_LOAD_VAR, // push a value from the variables map to the stack
    // Control flow
    OP_RETURN,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_FUNCTION_CALL,
    //Scope 
    OP_INC_SCOPE,
    OP_DEC_SCOPE,
    // Output
    OP_PRINT,
    // Stdlib
    OP_STD_LIB_CALL
};

// For athritmetic and comparison operations
std::unordered_map<std::string, OpCode> opCodeMap = { 
    {"+", OpCode::OP_ADD},
    {"-", OpCode::OP_SUB},
    {"*", OpCode::OP_MUL},
    {"/", OpCode::OP_DIV},
    {"==", OpCode::OP_EQ},
    {"!=", OpCode::OP_NEQ},
    {">", OpCode::OP_GT},
    {"<", OpCode::OP_LT},
    {">=", OpCode::OP_GTEQ},
    {"<=", OpCode::OP_LTEQ}
};


// Data Structures ---------------------------------------------------
struct function; // Forward declaration

struct function {
    int8_t* code; // Bytecode array
    int count;
    int capacity;

    std::string name; // Function name

    function* parent; // Parent function
    
    //arguments
    std::vector<std::string> arguments;

    std::map<std::string, function*> functions; // function map
};

std::map<std::string, function*> function_definitions; // Dynamically allocated because multiple functions can be created
                                                        // This vector stores the functions

enum Value_Type{
    NUMBER,
    BOOL,
    STRING,
    STRUCT,
};

struct Value{
    Value_Type type;
    std::variant<double, bool, std::string> data;
};

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
        default:
            return "UNKNOWN"; // Should never reach here, but to avoid warnings
    }
}

void print_value(Value value){
    std::cout << VALUE_AS_STRING(value);
}


// Constants struct
// This is so constants can be stored in the bytecode as indices
// The array will probably get changed to be Values instead of doubles
struct values{
    Value* values;
    int count;
    int capacity;
};

values vals; // Statically allocated because only one values array is needed
             // This array stores constant values

// Variable names struct
// This is so variable names can be stored in the bytecode as indices
struct varible_names{
    std::string* names;
    int count;
    int capacity;
};

varible_names variable_names; // Statically allocated because only one variable names array is needed
                              // This array stores the names of the variables
// -------------------------------------------------------------------

// Visual Representation for debugging -------------------------------
void display_bytecode(function* func){
    for(int i = 0; i < func->count; i++){
        std::cout << i << ": ";
        switch(func->code[i]){
            // Arithmetic
            case OpCode::OP_ADD:
                std::cout << "OP_ADD" << std::endl;
                break;
            case OpCode::OP_SUB:
                std::cout << "OP_SUB" << std::endl;
                break;
            case OpCode::OP_MUL:
                std::cout << "OP_MUL" << std::endl;
                break;
            case OpCode::OP_DIV:
                std::cout << "OP_DIV" << std::endl;
                break;

            // Comparison
            case OpCode::OP_EQ:
                std::cout << "OP_EQ" << std::endl;
                break;
            case OpCode::OP_NEQ:
                std::cout << "OP_NEQ" << std::endl;
                break;
            case OpCode::OP_GT: 
                std::cout << "OP_GT" << std::endl;
                break;
            case OpCode::OP_LT:
                std::cout << "OP_LT" << std::endl;
                break;
            case OpCode::OP_GTEQ:
                std::cout << "OP_GTEQ" << std::endl;
                break;
            case OpCode::OP_LTEQ:
                std::cout << "OP_LTEQ" << std::endl;
                break;

            // Variables
            case OpCode::OP_LOAD:
                std::cout << "OP_LOAD";
                std::cout << "          ";
                std::cout << "Index: " << (int)func->code[++i];
                std::cout << "          ";
                std::cout << "Value: " << VALUE_AS_STRING(vals.values[(int)func->code[i]]) << std::endl;
                break;
            case OpCode::OP_STORE_VAR:
                std::cout << "OP_STORE_VAR";
                std::cout << "          ";
                std::cout << "Index: " << (int)func->code[++i];
                std::cout << "          ";
                std::cout << "Name: " << variable_names.names[(int)func->code[i]] << std::endl;
                break;
            case OpCode::OP_UPDATE_VAR:
                std::cout << "OP_UPDATE_VAR";
                std::cout << "          ";
                std::cout << "Index: " << (int)func->code[++i];
                std::cout << "          ";
                std::cout << "Name: " << variable_names.names[(int)func->code[i]] << std::endl;
                break;
            case OpCode::OP_LOAD_VAR:
                std::cout << "OP_LOAD_VAR";
                std::cout << "          ";
                std::cout << "Index: " << (int)func->code[++i];
                std::cout << "          ";
                std::cout << "Name: " << variable_names.names[(int)func->code[i]] << std::endl;
                break;
            
            // Control flow
            case OpCode::OP_RETURN:
                std::cout << "OP_RETURN" << std::endl;
                break;
            case OpCode::OP_JUMP:
                std::cout << "OP_JUMP    " << "Offset: " << (int)func->code[++i] << std::endl;
                break;
            case OpCode::OP_JUMP_IF_FALSE:
                std::cout << "OP_JUMP_IF_FALSE    " << "Offset: " << (int)func->code[++i] << std::endl;
                break;
            case OpCode::OP_FUNCTION_CALL:
                std::cout << "OP_FUNCTION_CALL";
                std::cout << "          ";
                std::cout << "Index: " << (int)func->code[++i];
                std::cout << "          ";
                std::cout << "Name: " << variable_names.names[(int)func->code[i]] << std::endl;
                break;
            
            // Scope
            case OpCode::OP_INC_SCOPE:
                std::cout << "OP_INC_SCOPE" << std::endl;
                break;
            case OpCode::OP_DEC_SCOPE:
                std::cout << "OP_DEC_SCOPE" << std::endl;
                break;
            
            // Output
            case OpCode::OP_PRINT:
                std::cout << "OP_PRINT" << std::endl;
                break;

            // Stdlib
            case OpCode::OP_STD_LIB_CALL:
                std::cout << "OP_STD_LIB_CALL";
                std::cout << "          ";
                std::cout << "Index: " << (int)func->code[++i];
                std::cout << "          ";
                std::cout << "Name: " << STD_LIB_FUNCTION_NAMES[(int)func->code[i]] << std::endl;                
                break;

            default:
                std::cout << "Unknown opcode" << std::endl;
                break;
        }
    }
}

void display_constants(){
    for(int i = 0; i < vals.count; i++){
        std::cout << i << ": " << VALUE_AS_STRING(vals.values[i]) << std::endl;
    }
}

void display_variables(){
    for(int i = 0; i < variable_names.count; i++){
        std::cout << i << ": " << variable_names.names[i] << std::endl;
    }
}
// -------------------------------------------------------------------

// Helper functions --------------------------------------------------
inline void WRITE_BYTE(int8_t byte, function* func){
    func->code[func->count++] = byte;
}

inline void WRITE_VALUE(double value){
    vals.values[vals.count++] = {NUMBER, value};
}

inline void WRITE_VALUE(bool value){
    vals.values[vals.count++] = {BOOL, value};
}

inline void WRITE_VALUE(const std::string& value){
    vals.values[vals.count++] = {STRING, value};
}

inline void WRITE_VALUE(Value value){
    vals.values[vals.count++] = value;
}

inline void WRITE_VAR_NAME(const std::string& name){
    variable_names.names[variable_names.count++] = name;
}

int get_variable_index(const std::string& name){ // returns the index of the variable in the variable names array
    for(int i = 0; i < variable_names.count; i++){
        if(variable_names.names[i] == name){
            return i;
        }
    }
    return -1;
}

std::string get_variable_name(int index){
    return variable_names.names[index];
}

// Constructor for the function struct
function* create_function(int capacity, std::string name = "", function* parent = nullptr){
    function* func = new function;
    func->code = new int8_t[capacity]; 
    func->count = 0;
    func->capacity = capacity;

    func->parent = parent; // Set the parent function

    if(name == "main"){
        func->name = name;
        return func;
    }
    else{
        func->name = name + "^" + parent->name;
    }

    // std::cout << "Function name: " << name << std::endl;
    // std::cout << "Parent function name: " << parent->name << std::endl;

    if(parent != nullptr){
        // new name should be function^parent_name to avoid name conflicts 
        // if there are name conflicts, the function will be overwritten, which is the intended behavior
        function_definitions[func->name] = func;
        //add the function to the functions map of the parent function
        parent->functions[func->name] = func;
        //add the function name to the variables map, because functions are variables
        WRITE_VAR_NAME(func->name);
    }

    return func;
}

Value get_constant(int index){
    return vals.values[index];
}
// -------------------------------------------------------------------

// Interpretation ----------------------------------------------------
void interpret(Node* node, function* func);
void interpret_stmt_list(Node* node, function* func);
void interpret_stmt(Node* node, function* func);
void interpret_if(Node* node, function* func);
void interpret_return(Node* node, function* func);
void interpret_expr(Node* node, function* func);
void interpret_op(Node* node, function* func);

void interpretation_error(std::string message, Node* node){
    std::cout << message << std::endl;
    node->print();
    exit(1);
}

void interpret_function_call(Node* node, function* func){
    if(node->get_type() != NodeType::FUNCTION_CALL_NODE){
        interpretation_error("Function call doesn't start with FUNCTION_CALL Node", node);
    }

    //get the name of the function
    std::string name = node->get_value(1);

    //look up the function scopes to find the function
    //finds definition that is closest to the current function
    function* func_where_called_func_resides = func;
    while(func_where_called_func_resides != nullptr){
        if(func_where_called_func_resides->functions.find(name + '^' + func_where_called_func_resides->name) != func_where_called_func_resides->functions.end()){
            break;
        }
        func_where_called_func_resides = func_where_called_func_resides->parent;
    }

    if(func_where_called_func_resides == nullptr){
        interpretation_error("Function not found: " + name, nullptr);
    }

    name = name + '^' + func_where_called_func_resides->name;

    // Get the function from the variables map
    // lookup the function in the functions map
    function* called_func = func_where_called_func_resides->functions[name];
    if(called_func == nullptr){
        interpretation_error("Function not found: " + name, nullptr);
    }

    //check if the number of arguments is correct
    if(node->get_child(0)->get_children().size() != called_func->arguments.size()){
        interpretation_error("Incorrect number of arguments", node);
    }
    //push the arguments to the stack
    for(int i = 0; i < (int)node->get_child(0)->get_children().size(); i++){
        interpret_expr(node->get_child(0)->get_child(i), func);
    }

    //call the function
    WRITE_BYTE(OpCode::OP_FUNCTION_CALL, func);
    //write the function index
    if(get_variable_index(name) == -1){
        interpretation_error("Function not found: " + name, node);
    }
    WRITE_BYTE(get_variable_index(name), func);
}

void interpret_std_lib_call(Node* node, function* func){
    if(node->get_type() != NodeType::STD_LIB_CALL_NODE){
        interpretation_error("Std Lib call doesn't start with STD_LIB_CALL Node", node);
    }

    //get the name of the function
    std::string name = node->get_value(1);

    //push the arguments to the stack
    for(int i = 0; i < (int)node->get_child(0)->get_children().size(); i++){
        interpret_expr(node->get_child(0)->get_child(i), func);
    }

    WRITE_BYTE(OpCode::OP_STD_LIB_CALL, func);
    // look up the function in the std lib function names array
    for(int i = 0; i < (int)STD_LIB_FUNCTION_NAMES.size(); i++){
        if(name == STD_LIB_FUNCTION_NAMES[i]){
            WRITE_BYTE(i, func);
            return;
        }
    }
    interpretation_error("Std Lib function not found: " + name, node);
}

void choose_expr_operand(Node* node, function* func){
    switch(node->get_type()){
            case NodeType::OP_NODE:
                interpret_op(node, func);
                break;
            case NodeType::NUM_NODE:
                //vals.values[vals.count++] = std::stod(l_child->get_value());
                WRITE_VALUE(std::stod(node->get_value()));
                WRITE_BYTE(OpCode::OP_LOAD, func);
                WRITE_BYTE(vals.count - 1, func);
                break;
            case NodeType::VAR_NODE:
                WRITE_BYTE(OpCode::OP_LOAD_VAR, func);
                if(get_variable_index(node->get_value()) == -1){
                    interpretation_error("Variable not found", node);
                }
                WRITE_BYTE(get_variable_index(node->get_value()), func);
                break;
            case NodeType::FUNCTION_CALL_NODE:
                interpret_function_call(node, func);
                break;
            case NodeType::STD_LIB_CALL_NODE:
                interpret_std_lib_call(node, func);
                break;
            case NodeType::EXPR_NODE:
                interpret_expr(node, func);
                break;
            case NodeType::STRING_NODE:
                //std::cout << l_child->get_value() << std::endl;
                WRITE_VALUE(node->get_value());
                WRITE_BYTE(OpCode::OP_LOAD, func);
                WRITE_BYTE(vals.count - 1, func);
                break;
            default:
                interpretation_error("Invalid child type for OP Node", node);
                break;
        }
}

void interpret_op(Node* node, function* func){
    if(node->get_type() == NodeType::OP_NODE){
        Node* l_child = node->get_child(0);
        Node* r_child = node->get_child(1);

        choose_expr_operand(l_child, func);

        choose_expr_operand(r_child, func);

        std::string opStr = node->get_value();
        
        if(opCodeMap.find(opStr) == opCodeMap.end()){
            interpretation_error("Invalid operator", node);
        }

        WRITE_BYTE(opCodeMap[opStr], func);
    }
    else{
        interpretation_error("Operator doesn't start with OP Node", node);
    }
}

void interpret_expr(Node* node, function* func){
    if(node->get_type() == NodeType::EXPR_NODE){
        choose_expr_operand(node->get_child(0), func);
    }
    else{
        interpretation_error("Expression doesn't start with EXPR Node", node);
    }
}

void interpret_return(Node* node, function* func){
    if(node->get_type() != NodeType::RETURN_NODE){
        interpretation_error("Return doesn't start with RETURN Node", node);
    }

    interpret_expr(node->get_child(0), func); // Expression to return

    WRITE_BYTE(OpCode::OP_RETURN, func);
}

void interpret_if(Node* node, function* func){
    if(node->get_type() != NodeType::IF_NODE){
        interpretation_error("If doesn't start with IF Node", node);
    }

    interpret_expr(node->get_child(0), func);
    WRITE_BYTE(OpCode::OP_JUMP_IF_FALSE, func);
    int jump_index = func->count;
    WRITE_BYTE(0, func); // Placeholder for the jump index

    WRITE_BYTE(OpCode::OP_INC_SCOPE, func); // Increase the scope for the if block

    interpret_stmt_list(node->get_child(1), func);

    WRITE_BYTE(OpCode::OP_DEC_SCOPE, func); // Decrease the scope for the if block

    int jump_offset = func->count - jump_index;
    func->code[jump_index] = jump_offset;

    // check if there is an else block
    if(node->get_children().size() == 3){
        func->code[jump_index] = jump_offset + 2; 

        WRITE_BYTE(OpCode::OP_JUMP, func);
        int jump_index = func->count;
        WRITE_BYTE(0, func); // Placeholder for the jump index

        WRITE_BYTE(OpCode::OP_INC_SCOPE, func); // Increase the scope for the else block

        interpret_stmt_list(node->get_child(2), func);

        WRITE_BYTE(OpCode::OP_DEC_SCOPE, func); // Decrease the scope for the else block

        int jump_offset = func->count - jump_index;
        func->code[jump_index] = jump_offset;
    }
}

void interpret_function(Node* node, function* func, std::string name){
    if(node->get_type() != NodeType::FUNCTION_NODE){
        interpretation_error("Function doesn't start with FUNCTION Node", node);
    }

    function* new_func = create_function(1000, name, func);

    // add the arguments to the variables map
    for(int i = 0; i < (int)node->get_child(0)->get_children().size(); i++){
        std::string arg_name = node->get_child(0)->get_child(i)->get_value();
        WRITE_VAR_NAME(arg_name);

        new_func->arguments.push_back(arg_name);
    }

    //assign the stack values to the arguments
    for(int i = new_func->arguments.size() - 1; i >= 0 ; i--){ // reverse loop to keep the order of the arguments
        WRITE_BYTE(OpCode::OP_STORE_VAR, new_func);
        WRITE_BYTE(i + variable_names.count - new_func->arguments.size(), new_func);
    }

    //make sure the function isn't empty
    if(node->get_children().size() == 0){
        interpretation_error("Function is empty", node);
    }
    interpret_stmt_list(node->get_child(1), new_func);
}

void interpret_assign(Node* node, function* func){
    if(node->get_type() != NodeType::ASSIGN_NODE){
        interpretation_error("Assign doesn't start with ASSIGN Node", node);
    }

    if(node->get_child(0)->get_type() != NodeType::VAR_NODE){
        interpretation_error("Assign doesn't have a VAR Node as the first child", node);
    }

    if(node->get_child(1)->get_type() == NodeType::EXPR_NODE){ // Assigning a value to a variable
        interpret_expr(node->get_child(1), func);

        WRITE_BYTE(OpCode::OP_STORE_VAR, func); // takes the value from the stack and stores it in the variables map
        if(get_variable_index(node->get_child(0)->get_value()) == -1){ // if the variable doesn't exist, add it to the variable names array
            WRITE_VAR_NAME(node->get_child(0)->get_value());
        }
        WRITE_BYTE(get_variable_index(node->get_child(0)->get_value()), func);
    }
    else if(node->get_child(1)->get_type() == NodeType::FUNCTION_NODE){ // Assigning a function to a variable
        interpret_function(node->get_child(1), func, std::string(node->get_child(0)->get_value()));
    }
    else{
        interpretation_error("Invalid child type for ASSIGN Node", node);
    }
}

void interpret_update(Node* node, function* func){
    if(node->get_type() != NodeType::UPDATE_NODE){
        interpretation_error("Update doesn't start with UPDATE Node", node);
    }

    if(node->get_child(0)->get_type() != NodeType::VAR_NODE){
        interpretation_error("Update doesn't have a VAR Node as the first child", node);
    }

    interpret_expr(node->get_child(1), func); // Expression to update variable with 

    WRITE_BYTE(OpCode::OP_UPDATE_VAR, func); // takes the value from the stack and updates the value in the variables map
    int index = get_variable_index(node->get_child(0)->get_value());
    if(index == -1){
        // Should never happen because the variable should already exist
        interpretation_error("Trying to update a variable that hasn't been defined", node);
    }
    WRITE_BYTE(index, func);
}

void interpret_print(Node* node, function* func){
    if(node->get_type() != NodeType::PRINT_NODE){
        interpretation_error("Print doesn't start with PRINT Node", node);
    }

    interpret_expr(node->get_child(0), func); // Expression to print

    WRITE_BYTE(OpCode::OP_PRINT, func);
}

void interpret_for(Node* node, function* func){
    if(node->get_type() != NodeType::FOR_NODE){
        interpretation_error("For doesn't start with FOR Node", node);
    }

    WRITE_BYTE(OpCode::OP_INC_SCOPE, func); // Increase the scope for the for loop

    interpret_assign(node->get_child(0), func); // Initialize the for loop

    // get the index to jump back to
    int jump_index = func->count;

    // interpret the condition
    interpret_expr(node->get_child(1), func);

    // jump if the condition is false
    WRITE_BYTE(OpCode::OP_JUMP_IF_FALSE, func);

    // get the index to jump to the end of the for loop
    int jump_index2 = func->count;
    WRITE_BYTE(0, func); // Placeholder for the jump index

    interpret_stmt_list(node->get_child(3), func); // Interpret the body of the for loop

    interpret_update(node->get_child(2), func); // Update the for loop

    // jump back to the condition
    WRITE_BYTE(OpCode::OP_JUMP, func);
    WRITE_BYTE(jump_index - func->count, func);

    // jump to the end of the for loop
    int jump_offset = func->count - jump_index2;
    func->code[jump_index2] = jump_offset;

    WRITE_BYTE(OpCode::OP_DEC_SCOPE, func); // Decrease the scope for the for loop
}

void interpret_stmt(Node* node, function* func){
    if(node->get_type() == NodeType::STMT_NODE){
        Node* child = node->get_child(0);
        switch(child->get_type()){
            case NodeType::EXPR_NODE:
                interpret_expr(child, func);
                break;
            case NodeType::RETURN_NODE:
                interpret_return(child, func);
                break;
            case NodeType::IF_NODE:
                interpret_if(child, func);
                break;
            case NodeType::ASSIGN_NODE:
                interpret_assign(child, func);
                break;
            case NodeType::UPDATE_NODE:
                interpret_update(child, func);
                break;
            case NodeType::PRINT_NODE:
                interpret_print(child, func);
                break;
            case NodeType::FOR_NODE:
                interpret_for(child, func);
                break;
            default:
                interpretation_error("Invalid statement type", node);
                break;
        }
    }
    else{
        interpretation_error("Statement doesn't start with STMT Node", node);
    }
}

void interpret_stmt_list(Node* node, function* func){
    if(node->get_children().size() == 0){
        return;
    }

    if(node->get_type() != NodeType::STMT_LIST_NODE){
        interpretation_error("Statement List doesn't start with STMT_LIST Node", node);
    }

    interpret_stmt(node->get_child(0), func);
    if(node->get_children().size() == 2){
        interpret_stmt_list(node->get_child(1), func);
    }
}

void interpret(Node* node, function* func){
    if(node->get_type() != NodeType::STMT_LIST_NODE){
        interpretation_error("Program doesn't start with STMT_LIST Node", node);
    }
    
    interpret_stmt_list(node, func);
}

void generate_bytecode(Node* ast) {
    function* func = create_function(1000, "main");

    function_definitions["main"] = func;

    vals.values = new Value[255]; // Allocate 1000 doubles for the values
                                    // this is the maximum number of constants because it fits into a byte for the bytecode
    vals.count = 0;
    vals.capacity = 255;

    variable_names.names = new std::string[255]; // Allocate 255 strings for the variable names
                                                  // this is the maximum number of variables because it fits into a byte for the bytecode
    variable_names.count = 0;
    variable_names.capacity = 255;

    interpret(ast, func);
}

// -------------------------------------------------------------------

#endif // BYTECODE_GENERATOR_HPP