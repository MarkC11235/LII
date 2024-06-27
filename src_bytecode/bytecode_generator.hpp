#ifndef BYTECODE_GENERATOR_HPP
#define BYTECODE_GENERATOR_HPP

#include <cstdint> // int8_t
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "parser.hpp"

enum OpCode{
    // Arithmetic
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
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
    OP_PRINT
};


// Data Structures ---------------------------------------------------
struct function;

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

struct values{
    double* values;
    int count;
    int capacity;
};

values vals; // Statically allocated because only one values array is needed
             // This array stores constant values

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

            // Variables
            case OpCode::OP_LOAD:
                std::cout << "OP_LOAD";
                std::cout << "          ";
                std::cout << "Index: " << (int)func->code[++i];
                std::cout << "          ";
                std::cout << "Value: " << vals.values[(int)func->code[i]] << std::endl;
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


            default:
                std::cout << "Unknown opcode" << std::endl;
                break;
        }
    }
}

void display_constants(){
    for(int i = 0; i < vals.count; i++){
        std::cout << i << ": " << vals.values[i] << std::endl;
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
    vals.values[vals.count++] = value;
}

void WRITE_VAR_NAME(const std::string& name){
    variable_names.names[variable_names.count++] = name;
}

int get_variable_index(const std::string& name){
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

function* create_function(int capacity, std::string name = "", function* parent = nullptr){
    function* func = new function;
    func->code = new int8_t[capacity];
    func->count = 0;
    func->capacity = capacity;

    func->parent = parent;

    func->name = name;

    if(parent != nullptr){
        //add the function to the functions map of the parent function
        parent->functions[name] = func;
        //add the function name to the variables map
        WRITE_VAR_NAME(name);
    }

    return func;
}

double get_constant(int index){
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
    if(node->get_type() != NodeType::FUNCTION_CALL){
        interpretation_error("Function call doesn't start with FUNCTION_CALL Node", node);
    }

    //get the name of the function
    std::string name = node->get_value(1);
    //std::cout << name << std::endl;

    //look up the function scopes to find the function
    function* func_where_called_func_resides = func;
    while(func_where_called_func_resides != nullptr){
        if(func_where_called_func_resides->functions.find(name) != func_where_called_func_resides->functions.end()){
            break;
        }
        func_where_called_func_resides = func_where_called_func_resides->parent;
    }

    //std::cout << "Current function where called function resides : " << func_where_called_func_resides->name << std::endl;

    if(func_where_called_func_resides == nullptr){
        interpretation_error("Function not found: " + name, nullptr);
    }

    // Get the function from the variables map
    // lookup the function in the functions map
    //std::cout << "Function name: " << name << std::endl;
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

void interpret_op(Node* node, function* func){
    if(node->get_type() == NodeType::OP){
        Node* l_child = node->get_child(0);
        Node* r_child = node->get_child(1);

        // if(l_child->get_type() == NodeType::OP){
        //     interpret_op(l_child, func);
        // }
        // else if(l_child->get_type() == NodeType::NUM){
        //     vals.values[vals.count++] = std::stod(l_child->get_value());
        //     //bc.code[bc.count++] = OpCode::OP_LOAD;
        //     //bc.code[bc.count++] = vals.count - 1;
        //     WRITE_BYTE(OpCode::OP_LOAD, func);
        //     WRITE_BYTE(vals.count - 1, func);
        // }
        // else if(l_child->get_type() == NodeType::VAR){
        //     WRITE_BYTE(OpCode::OP_LOAD_VAR, func);
        //     if(get_variable_index(l_child->get_value()) == -1){
        //         interpretation_error("Variable not found", node);
        //     }
        //     WRITE_BYTE(get_variable_index(l_child->get_value()), func);
        // }
        // else if(l_child->get_type() == NodeType::FUNCTION_CALL){
        //     interpret_function_call(l_child, func);
        // }
        // else{
        //     interpretation_error("Invalid child type for OP Node", node);
        // }

        switch(l_child->get_type()){
            case NodeType::OP:
                interpret_op(l_child, func);
                break;
            case NodeType::NUM:
                vals.values[vals.count++] = std::stod(l_child->get_value());
                WRITE_BYTE(OpCode::OP_LOAD, func);
                WRITE_BYTE(vals.count - 1, func);
                break;
            case NodeType::VAR:
                WRITE_BYTE(OpCode::OP_LOAD_VAR, func);
                if(get_variable_index(l_child->get_value()) == -1){
                    interpretation_error("Variable not found", node);
                }
                WRITE_BYTE(get_variable_index(l_child->get_value()), func);
                break;
            case NodeType::FUNCTION_CALL:
                interpret_function_call(l_child, func);
                break;
            case NodeType::EXPR:
                interpret_expr(l_child, func);
                break;
            default:
                interpretation_error("Invalid child type for OP Node", node);
                break;
        }

        // if(r_child->get_type() == NodeType::OP){
        //     interpret_op(r_child, func);
        // }
        // else if(r_child->get_type() == NodeType::NUM){
        //     vals.values[vals.count++] = std::stod(r_child->get_value());
        //     // bc.code[bc.count++] = OpCode::OP_LOAD;
        //     // bc.code[bc.count++] = vals.count - 1;
        //     WRITE_BYTE(OpCode::OP_LOAD, func);
        //     WRITE_BYTE(vals.count - 1, func);
        // }
        // else if(r_child->get_type() == NodeType::VAR){
        //     WRITE_BYTE(OpCode::OP_LOAD_VAR, func);
        //     if(get_variable_index(r_child->get_value()) == -1){
        //         interpretation_error("Variable not found", node);
        //     }
        //     WRITE_BYTE(get_variable_index(r_child->get_value()), func);
        // }
        // else if(r_child->get_type() == NodeType::FUNCTION_CALL){
        //     interpret_function_call(r_child, func);
        // }
        // else{
        //     interpretation_error("Invalid child type for OP Node", node);
        // }

        switch(r_child->get_type()){
            case NodeType::OP:
                interpret_op(r_child, func);
                break;
            case NodeType::NUM:
                vals.values[vals.count++] = std::stod(r_child->get_value());
                WRITE_BYTE(OpCode::OP_LOAD, func);
                WRITE_BYTE(vals.count - 1, func);
                break;
            case NodeType::VAR:
                WRITE_BYTE(OpCode::OP_LOAD_VAR, func);
                if(get_variable_index(r_child->get_value()) == -1){
                    interpretation_error("Variable not found", node);
                }
                WRITE_BYTE(get_variable_index(r_child->get_value()), func);
                break;
            case NodeType::FUNCTION_CALL:
                interpret_function_call(r_child, func);
                break;
            case NodeType::EXPR:
                interpret_expr(r_child, func);
                break;
            default:
                interpretation_error("Invalid child type for OP Node", node);
                break;
        }
        
        switch(node->get_value()[0]){
            case '+':
                //bc.code[bc.count++] = OpCode::OP_ADD;
                WRITE_BYTE(OpCode::OP_ADD, func);
                break;
            case '-':
                //bc.code[bc.count++] = OpCode::OP_SUB;
                WRITE_BYTE(OpCode::OP_SUB, func);
                break;
            case '*':
                //bc.code[bc.count++] = OpCode::OP_MUL;
                WRITE_BYTE(OpCode::OP_MUL, func);
                break;
            case '/':
                //bc.code[bc.count++] = OpCode::OP_DIV;
                WRITE_BYTE(OpCode::OP_DIV, func);
                break;
            default:
                interpretation_error("Invalid operator", node);
                break;
        }
    }
    else{
        interpretation_error("Operator doesn't start with OP Node", node);
    }
}

void interpret_expr(Node* node, function* func){
    if(node->get_type() == NodeType::EXPR){
        if(node->get_child(0)->get_type() == NodeType::OP){
            interpret_op(node->get_child(0), func);
        }
        else if(node->get_child(0)->get_type() == NodeType::NUM){
            vals.values[vals.count++] = std::stod(node->get_child(0)->get_value());
            WRITE_BYTE(OpCode::OP_LOAD, func);
            WRITE_BYTE(vals.count - 1, func);
        }
        else if(node->get_child(0)->get_type() == NodeType::VAR){
            WRITE_BYTE(OpCode::OP_LOAD_VAR, func);
            if(get_variable_index(node->get_child(0)->get_value()) == -1){
                interpretation_error("Variable not found", node);
            }
            WRITE_BYTE(get_variable_index(node->get_child(0)->get_value()), func);
        }
        else if(node->get_child(0)->get_type() == NodeType::FUNCTION_CALL){
            interpret_function_call(node->get_child(0), func);
        }
        else{
            interpretation_error("Expression doesn't start with OP Node", node);
        }
    }
    else{
        interpretation_error("Expression doesn't start with EXPR Node", node);
    }
}

void interpret_return(Node* node, function* func){
    if(node->get_type() != NodeType::RETURN){
        interpretation_error("Return doesn't start with RETURN Node", node);
    }

    interpret_expr(node->get_child(0), func);

    //bc.code[bc.count++] = OpCode::OP_RETURN;
    WRITE_BYTE(OpCode::OP_RETURN, func);
}

void interpret_if(Node* node, function* func){
    if(node->get_type() != NodeType::IF){
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
    if(node->get_type() != NodeType::FUNCTION){
        interpretation_error("Function doesn't start with FUNCTION Node", node);
    }

    function* new_func = create_function(1000, name, func);

    function_definitions[name] = new_func;

    // add the arguments to the variables map
    for(int i = 0; i < (int)node->get_child(0)->get_children().size(); i++){
        std::string arg_name = node->get_child(0)->get_child(i)->get_value();
        //std::cout << "arg_name: " << arg_name << std::endl;
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
    if(node->get_type() != NodeType::ASSIGN){
        interpretation_error("Assign doesn't start with ASSIGN Node", node);
    }

    if(node->get_child(0)->get_type() != NodeType::VAR){
        interpretation_error("Assign doesn't have a VAR Node as the first child", node);
    }

    if(node->get_child(1)->get_type() == NodeType::EXPR){
        interpret_expr(node->get_child(1), func);

        WRITE_BYTE(OpCode::OP_STORE_VAR, func); // takes the value from the stack and stores it in the variables map
        if(get_variable_index(node->get_child(0)->get_value()) == -1){
            WRITE_VAR_NAME(node->get_child(0)->get_value());
            // print the variables map
            // for(int i = 0; i < variable_names.count; i++){
            //     std::cout << variable_names.names[i] << std::endl;
            // }
        }
        WRITE_BYTE(get_variable_index(node->get_child(0)->get_value()), func);
    }
    else if(node->get_child(1)->get_type() == NodeType::FUNCTION){
        interpret_function(node->get_child(1), func, std::string(node->get_child(0)->get_value()));
    }
    else{
        interpretation_error("Invalid child type for ASSIGN Node", node);
    }
}

void interpret_update(Node* node, function* func){
    if(node->get_type() != NodeType::UPDATE){
        interpretation_error("Update doesn't start with UPDATE Node", node);
    }

    if(node->get_child(0)->get_type() != NodeType::VAR){
        interpretation_error("Update doesn't have a VAR Node as the first child", node);
    }

    interpret_expr(node->get_child(1), func);

    WRITE_BYTE(OpCode::OP_UPDATE_VAR, func); // takes the value from the stack and updates the value in the variables map
    if(get_variable_index(node->get_child(0)->get_value()) == -1){
        
    }
    WRITE_BYTE(get_variable_index(node->get_child(0)->get_value()), func);
}

void interpret_print(Node* node, function* func){
    if(node->get_type() != NodeType::PRINT){
        interpretation_error("Print doesn't start with PRINT Node", node);
    }

    interpret_expr(node->get_child(0), func);

    WRITE_BYTE(OpCode::OP_PRINT, func);
}

void interpret_for(Node* node, function* func){
    if(node->get_type() != NodeType::FOR){
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
    if(node->get_type() == NodeType::STMT){
        Node* child = node->get_child(0);
        switch(child->get_type()){
            case NodeType::EXPR:
                interpret_expr(child, func);
                break;
            case NodeType::RETURN:
                interpret_return(child, func);
                break;
            case NodeType::IF:
                interpret_if(child, func);
                break;
            case NodeType::ASSIGN:
                interpret_assign(child, func);
                break;
            case NodeType::UPDATE:
                interpret_update(child, func);
                break;
            case NodeType::PRINT:
                interpret_print(child, func);
                break;
            case NodeType::FOR:
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

    //print the function's functions map
    // for(auto it = func->functions.begin(); it != func->functions.end(); it++){
    //     std::cout << it->first << std::endl;
    // }
}

void interpret_stmt_list(Node* node, function* func){
    if(node->get_children().size() == 0){
        return;
    }

    if(node->get_type() != NodeType::STMT_LIST){
        interpretation_error("Statement List doesn't start with STMT_LIST Node", node);
    }

    interpret_stmt(node->get_child(0), func);
    if(node->get_children().size() == 2){
        interpret_stmt_list(node->get_child(1), func);
    }
}

void interpret(Node* node, function* func){
    if(node->get_type() != NodeType::STMT_LIST){
        interpretation_error("Program doesn't start with STMT_LIST Node", node);
    }
    
    interpret_stmt_list(node, func);
}

void generate_bytecode(Node* ast) {
    function* func = create_function(1000, "main");

    function_definitions["main"] = func;

    vals.values = new double[255]; // Allocate 1000 doubles for the values
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