#ifndef BYTECODE_GENERATOR_HPP
#define BYTECODE_GENERATOR_HPP

#include <cstdint> // int8_t
#include <limits>  // std::numeric_limits
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <unordered_map>

#include "parser.hpp"
#include "Function.hpp"
#include "Value.hpp"
#include "./std_lib/std_lib.hpp"
#include "cl_exe_file.hpp"
#include "opcodes.hpp"

// For athritmetic and comparison operations
std::unordered_map<std::string, OpCode> opCodeMap = {
    {"[", OpCode::OP_ACCESS},
    {"u-", OpCode::OP_U_SUB},
    {"+", OpCode::OP_ADD},
    {"-", OpCode::OP_SUB},
    {"*", OpCode::OP_MUL},
    {"/", OpCode::OP_DIV},
    {"%", OpCode::OP_MOD},
    {"==", OpCode::OP_EQ},
    {"!=", OpCode::OP_NEQ},
    {">", OpCode::OP_GT},
    {"<", OpCode::OP_LT},
    {">=", OpCode::OP_GTEQ},
    {"<=", OpCode::OP_LTEQ},
    {"&&", OpCode::OP_AND},
    {"||", OpCode::OP_OR},
    {"!", OpCode::OP_NOT}};

// Data Structures ---------------------------------------------------
std::vector<Value> constants; // Statically allocated because only one constants array is needed
                              // This array stores constant values Ex: let x = 5; 5 is a constant

std::vector<std::string> variable_names; // Statically allocated because only one variable names array is needed
                                         // This array stores the names of the variables, functions are included in this array
// -------------------------------------------------------------------

// Visual Representation for debugging -------------------------------
void display_bytecode(function *func)
{
    for (int i = 0; i < func->count; i++)
    {
        std::cout << i << ": ";
        switch (func->code[i])
        {
        // Arithmetic
        case OpCode::OP_ADD:
            std::cout << "OP_ADD" << std::endl;
            break;
        case OpCode::OP_SUB:
            std::cout << "OP_SUB" << std::endl;
            break;
        case OpCode::OP_U_SUB:
            std::cout << "OP_U_SUB" << std::endl;
            break;
        case OpCode::OP_MUL:
            std::cout << "OP_MUL" << std::endl;
            break;
        case OpCode::OP_DIV:
            std::cout << "OP_DIV" << std::endl;
            break;
        case OpCode::OP_MOD:
            std::cout << "OP_MOD" << std::endl;
            break;

        // Boolean
        case OpCode::OP_AND:
            std::cout << "OP_AND" << std::endl;
            break;
        case OpCode::OP_OR:
            std::cout << "OP_OR" << std::endl;
            break;
        case OpCode::OP_NOT:
            std::cout << "OP_NOT" << std::endl;
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
            std::cout << "Value: " << VALUE_AS_STRING(constants[(int)func->code[i]]) << std::endl;
            break;
        case OpCode::OP_STORE_VAR:
            std::cout << "OP_STORE_VAR";
            std::cout << "          ";
            std::cout << "Index: " << (int)func->code[++i];
            std::cout << "          ";
            std::cout << "Name: " << variable_names[(int)func->code[i]] << std::endl;
            break;
        case OpCode::OP_UPDATE_VAR:
            std::cout << "OP_UPDATE_VAR";
            std::cout << "          ";
            std::cout << "Index: " << (int)func->code[++i];
            std::cout << "          ";
            std::cout << "Name: " << variable_names[(int)func->code[i]] << std::endl;
            break;
        case OpCode::OP_LOAD_VAR:
            std::cout << "OP_LOAD_VAR";
            std::cout << "          ";
            std::cout << "Index: " << (int)func->code[++i];
            std::cout << "          ";
            std::cout << "Name: " << variable_names[(int)func->code[i]] << std::endl;
            break;
        case OpCode::OP_LOAD_FUNCTION_VAR:
            std::cout << "OP_LOAD_FUNCTION_VAR";
            std::cout << "          ";
            std::cout << "Index: " << (int)func->code[++i];
            std::cout << "          ";
            std::cout << "Name: " << variable_names[(int)func->code[i]] << std::endl;
            break;

        // Arrays
        case OpCode::OP_CREATE_VECTOR:
            std::cout << "OP_CREATE_VECTOR" << std::endl;
            break;
        case OpCode::OP_VECTOR_PUSH:
            std::cout << "OP_VECTOR_PUSH";
            std::cout << "          ";
            std::cout << "Name: " << variable_names[(int)func->code[++i]] << std::endl;
            break;
        // case OpCode::OP_LOAD_VECTOR_ELEMENT:
        //     std::cout << "OP_LOAD_VECTOR_ELEMENT";
        //     std::cout << "          ";
        //     std::cout << "Vector Name: " << variable_names[(int)func->code[++i]];
        //     std::cout << std::endl;
        //     break;
        case OpCode::OP_UPDATE_VECTOR_ELEMENT:
            std::cout << "OP_UPDATE_VECTOR_ELEMENT";
            std::cout << "          ";
            std::cout << "Vector Name: " << variable_names[(int)func->code[++i]];
            std::cout << std::endl;
            break;

        // Structs
        case OpCode::OP_CREATE_STRUCT:
            std::cout << "OP_CREATE_STRUCT" << std::endl;
            break;
        // case OpCode::OP_LOAD_STRUCT_ELEMENT:
        //     std::cout << "OP_LOAD_STRUCT_ELEMENT";
        //     std::cout << "          ";
        //     std::cout << "Struct Name: " << variable_names[(int)func->code[++i]];
        //     std::cout << "          ";
        //     std::cout << "Element Name: " << variable_names[(int)func->code[++i]];
        //     std::cout << std::endl;
        //     break;
        case OpCode::OP_UPDATE_STRUCT_ELEMENT:
            std::cout << "OP_UPDATE_STRUCT_ELEMENT";
            std::cout << "          ";
            std::cout << "Struct Name: " << variable_names[(int)func->code[++i]];
            std::cout << "          ";
            std::cout << "Element Name: " << variable_names[(int)func->code[++i]];
            std::cout << std::endl;
            break;
        case OpCode::OP_ACCESS:
            std::cout << "OP_ACCESS" << std::endl;
            break;
        case OpCode::OP_ACCESS_FOR_UPDATE:
            std::cout << "OP_ACCESS_FOR_UPDATE" << std::endl;
            break;
        case OpCode::OP_UPDATE_STACK_ELEMENT:
            std::cout << "OP_UPDATE_STACK_ELEMENT" << std::endl;
            break;

        // Control flow
        case OpCode::OP_RETURN:
            std::cout << "OP_RETURN" << std::endl;
            break;
        case OpCode::OP_JUMP:
            std::cout << "OP_JUMP    " << "Index: " << (int)func->code[++i] << std::endl;
            break;
        case OpCode::OP_JUMP_IF_FALSE:
            std::cout << "OP_JUMP_IF_FALSE    " << "Index: " << (int)func->code[++i] << std::endl;
            break;
        case OpCode::OP_FUNCTION_CALL:
            std::cout << "OP_FUNCTION_CALL" << std::endl;
            ;
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
            std::cout << "Name: " << STD_LIB_FUNCTIONS_DEFINITIONS[(int)func->code[i]].name << std::endl;
            break;

        default:
            std::cout << "Unknown opcode" << std::endl;
            break;
        }
    }
}

void display_constants()
{
    for (int i = 0; i < (int)constants.size(); i++)
    {
        std::cout << i << ": \n";
        print_value(constants[i]);
        std::cout << "\n"
                  << std::endl;
    }
}

void display_variables()
{
    for (int i = 0; i < (int)variable_names.size(); i++)
    {
        std::cout << i << ": " << variable_names[i] << std::endl;
    }
}
// -------------------------------------------------------------------

// Helper functions --------------------------------------------------
inline void WRITE_BYTE(CODE_SIZE byte, function *func)
{
    func->code[func->count++] = byte;
}

inline void CHANGE_BYTE(int index, CODE_SIZE byte, function *func)
{
    if (index >= func->count || index < 0)
    {
        std::cout << "Index out of bounds" << std::endl;
        exit(1);
    }
    func->code[index] = byte;
}

inline void FLAG_BYTE(int index, std::string flag, function *func)
{
    func->flags.push_back({index, flag});
}

inline void WRITE_VALUE(double value)
{
    constants.push_back({Value_Type::NUMBER, value});
}

inline void WRITE_VALUE(bool value)
{
    constants.push_back({Value_Type::BOOL, value});
}

inline void WRITE_VALUE(const std::string &value)
{
    constants.push_back({Value_Type::STRING, value});
}

inline void WRITE_VALUE(function *value)
{
    constants.push_back({Value_Type::FUNCTION, value});
}

inline void WRITE_VALUE(Value value)
{
    constants.push_back(value);
}

inline void WRITE_VAR_NAME(const std::string &name)
{
    variable_names.push_back(name);
}

int get_variable_index(const std::string &name)
{ // returns the index of the variable in the variable names array
    for (int i = 0; i < (int)variable_names.size(); i++)
    {
        if (variable_names[i] == name)
        {
            return i;
        }
    }
    return -1;
}

void WRITE_VAR_NAME_IF_NOT_EXISTS(const std::string &name)
{
    if (get_variable_index(name) == -1)
    {
        WRITE_VAR_NAME(name);
    }
}

std::string get_variable_name(int index)
{
    return variable_names[index];
}

// Constructor for the function struct
function *create_function(int capacity, function *parent = nullptr)
{
    function *func = new function;
    func->code = new CODE_SIZE[capacity];
    func->count = 0;
    func->capacity = capacity;

    return func;
}

inline Value get_constant(int index)
{
    return constants[index];
}
// -------------------------------------------------------------------

// Interpretation ----------------------------------------------------
void interpret(Node *node, function *func);
void interpret_stmt_list(Node *node, function *func);
void interpret_stmt(Node *node, function *func);
void interpret_if(Node *node, function *func);
void interpret_return(Node *node, function *func);
void interpret_expr(Node *node, function *func);
void interpret_op(Node *node, function *func);

void interpretation_error(std::string message, Node *node, function *func)
{
    std::cout << "Bytecode generation failed" << std::endl;

    std::cout << message << std::endl;
    node->print();

    // print the current state of the bytecode
    std::cout << "\nBytecode: " << std::endl;
    display_bytecode(func);
    std::cout << "\nConstants: " << std::endl;
    display_constants();
    std::cout << "\nVariable names: " << std::endl;
    display_variables();

    exit(1); // TODO: Handle errors better
}

void interpret_function_call(Node *node, function *func)
{
    if (node->get_type() != NodeType::FUNCTION_CALL_NODE)
    {
        interpretation_error("Function call doesn't start with FUNCTION_CALL Node", node, func);
    }

    // get the name of the function
    std::string name = node->get_value(1);

    // push the arguments to the stack
    Node *arg_list = node->get_child(0);
    for (int i = 0; i < (int)arg_list->get_children().size(); i++)
    {
        interpret_expr(arg_list->get_child(i), func);
    }

    // Push the function onto the stack
    WRITE_BYTE(OpCode::OP_LOAD_FUNCTION_VAR, func);
    if (get_variable_index(name) == -1)
    {
        interpretation_error("Function not found", node, func);
    }
    WRITE_BYTE(get_variable_index(name), func);

    // call the function
    WRITE_BYTE(OpCode::OP_FUNCTION_CALL, func);
}

void interpret_std_lib_call(Node *node, function *func)
{
    if (node->get_type() != NodeType::STD_LIB_CALL_NODE)
    {
        interpretation_error("Std Lib call doesn't start with STD_LIB_CALL Node", node, func);
    }

    std::string function_name = node->get_value(1);

    // push the arguments to the stack
    Node *arg_list = node->get_child(0);
    for (int i = 0; i < (int)arg_list->get_children().size(); i++)
    {
        interpret_expr(arg_list->get_child(i), func);
    }

    WRITE_BYTE(OpCode::OP_STD_LIB_CALL, func);
    // look up the function in the std lib function names array
    for (int i = 0; i < (int)STD_LIB_FUNCTIONS_DEFINITIONS.size(); i++)
    {
        if (function_name == STD_LIB_FUNCTIONS_DEFINITIONS[i].name)
        {
            WRITE_BYTE(i, func);
            return;
        }
    }
    interpretation_error("Std Lib function not found: " + function_name, node, func);
}

void choose_expr_operand(Node *node, function *func)
{
    std::string opStr = node->get_value();
    switch (node->get_type())
    {
    case NodeType::OP_NODE:
        interpret_op(node, func);
        break;
    case NodeType::NUM_NODE:
        WRITE_VALUE(std::stod(opStr));
        WRITE_BYTE(OpCode::OP_LOAD, func);
        WRITE_BYTE(constants.size() - 1, func);
        break;
    case NodeType::BOOL_NODE:
        WRITE_VALUE(opStr == "true"); // Convert the string to a bool
        WRITE_BYTE(OpCode::OP_LOAD, func);
        WRITE_BYTE(constants.size() - 1, func);
        break;
    case NodeType::VAR_NODE:
        if (node->get_children().size() == 0)
        { // Variable access
            WRITE_BYTE(OpCode::OP_LOAD_VAR, func);
            if (get_variable_index(opStr) == -1)
            {
                interpretation_error("Variable not found", node, func);
            }
            WRITE_BYTE(get_variable_index(node->get_value()), func);
        }
        // else if(node->get_children().size() == 1){ // Vector access or struct access
        //     Node* child = node->get_child(0);
        //     if(child->get_type() == NodeType::EXPR_NODE){ // Vector access
        //         interpret_expr(child, func); // Expression for vector index, first on the stack
        //         WRITE_BYTE(OpCode::OP_LOAD_VECTOR_ELEMENT, func); // Load the value from the vector
        //         if(get_variable_index(opStr) == -1){
        //             interpretation_error("Variable not found", node, func);
        //         }
        //         WRITE_BYTE(get_variable_index(opStr), func); // Index of the vector in the variables map
        //     }
        //     else if(child->get_type() == NodeType::VAR_NODE){ // Struct access
        //         WRITE_BYTE(OpCode::OP_LOAD_STRUCT_ELEMENT, func); // Load the value from the struct
        //         if(get_variable_index(node->get_value()) == -1){
        //             interpretation_error("Variable not found", node, func);
        //         }
        //         WRITE_BYTE(get_variable_index(opStr), func); // Index of the struct in the variables map
        //         if(get_variable_index(child->get_value()) == -1){
        //             interpretation_error("Variable not found", node, func);
        //         }
        //         WRITE_BYTE(get_variable_index(child->get_value()), func); // Index of the struct element in the struct
        //     }
        //     else{
        //         interpretation_error("Invalid child type for VAR Node", node, func);
        //     }
        // }
        else
        {
            interpretation_error("Invalid number of children for VAR Node", node, func);
        }
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
        WRITE_VALUE(node->get_value()); // Add the string to the constants array
        WRITE_BYTE(OpCode::OP_LOAD, func);
        WRITE_BYTE(constants.size() - 1, func);
        break;
    default:
        interpretation_error("Invalid child type for OP Node", node, func);
        break;
    }
}

void interpret_op(Node *node, function *func)
{
    if (node->get_type() == NodeType::OP_NODE)
    {
        std::string opStr = node->get_value();
        // std::cout << "opStr: " << opStr << std::endl;

        if (opCodeMap.find(opStr) == opCodeMap.end())
        {
            interpretation_error("Invalid operator", node, func);
        }

        if (operators.find(opStr) == operators.end())
        {
            interpretation_error("Operator not found", node, func);
        }

        if (std::get<1>(operators[opStr]) == "binary")
        {
            Node *l_child = node->get_child(0);
            choose_expr_operand(l_child, func);

            Node *r_child = node->get_child(1);
            choose_expr_operand(r_child, func);
        }
        else if (std::get<1>(operators[opStr]) == "unary")
        {
            Node *child = node->get_child(0);
            choose_expr_operand(child, func);
        }
        else if (std::get<1>(operators[opStr]) == "access")
        {
            Node *l_child = node->get_child(0);
            choose_expr_operand(l_child, func);

            Node *r_child = node->get_child(1);
            choose_expr_operand(r_child, func);
        }
        else
        {
            interpretation_error("Invalid operator type", node, func);
        }

        WRITE_BYTE(opCodeMap[opStr], func);
    }
    else
    {
        interpretation_error("Operator doesn't start with OP Node", node, func);
    }
}

void interpret_expr(Node *node, function *func)
{
    if (node->get_type() == NodeType::EXPR_NODE)
    {
        choose_expr_operand(node->get_child(0), func);
    }
    else
    {
        interpretation_error("Expression doesn't start with EXPR Node", node, func);
    }
}

void interpret_return(Node *node, function *func)
{
    if (node->get_type() != NodeType::RETURN_NODE)
    {
        interpretation_error("Return doesn't start with RETURN Node", node, func);
    }

    interpret_expr(node->get_child(0), func); // Expression to return

    WRITE_BYTE(OpCode::OP_RETURN, func);
}

void interpret_if(Node *node, function *func)
{
    if (node->get_type() != NodeType::IF_NODE)
    {
        interpretation_error("If doesn't start with IF Node", node, func);
    }

    interpret_expr(node->get_child(0), func);
    WRITE_BYTE(OpCode::OP_JUMP_IF_FALSE, func);
    WRITE_BYTE(0, func); // Placeholder for the jump index
    int jump_if_false_byte = func->count - 1;

    WRITE_BYTE(OpCode::OP_INC_SCOPE, func); // Increase the scope for the if block

    interpret_stmt_list(node->get_child(1), func);

    WRITE_BYTE(OpCode::OP_DEC_SCOPE, func); // Decrease the scope for the if block

    CHANGE_BYTE(jump_if_false_byte, func->count - 1, func); // Jump to the end of the if block

    // check if there is an else block
    if (node->get_children().size() == 3)
    {
        WRITE_BYTE(OpCode::OP_JUMP, func); // Jump to the end of the else block, because the if block was executed
        WRITE_BYTE(0, func);               // Placeholder for the jump index of the end of the else block
        int jump_byte = func->count - 1;

        CHANGE_BYTE(jump_if_false_byte, func->count - 1, func); // Jump to the else block

        WRITE_BYTE(OpCode::OP_INC_SCOPE, func); // Increase the scope for the else block

        interpret_stmt_list(node->get_child(2), func);

        WRITE_BYTE(OpCode::OP_DEC_SCOPE, func); // Decrease the scope for the else block

        CHANGE_BYTE(jump_byte, func->count - 1, func); // Jump to the end of the else block
    }
}

void interpret_function(Node *node, function *func, std::string name)
{
    if (node->get_type() != NodeType::FUNCTION_NODE)
    {
        interpretation_error("Function doesn't start with FUNCTION Node", node, func);
    }

    function *new_func = create_function(1000, func);
    WRITE_VALUE(new_func); // Add the function to the constants array

    WRITE_BYTE(OpCode::OP_LOAD, func); // push function pointer to stack
    WRITE_BYTE(constants.size() - 1, func);

    // add the arguments to the variables map
    for (int i = 0; i < (int)node->get_child(0)->get_children().size(); i++)
    {
        std::string arg_name = node->get_child(0)->get_child(i)->get_value();
        WRITE_VAR_NAME_IF_NOT_EXISTS(arg_name);

        new_func->arguments.push_back(arg_name);
    }

    // assign the stack values to the arguments
    for (int i = new_func->arguments.size() - 1; i >= 0; i--)
    { // reverse loop to keep the order of the arguments
        WRITE_BYTE(OpCode::OP_STORE_VAR, new_func);
        WRITE_BYTE(get_variable_index(new_func->arguments[i]), new_func);
    }

    // make sure the function isn't empty
    if (node->get_children().size() == 0)
    {
        interpretation_error("Function is empty", node, func);
    }
    interpret_stmt_list(node->get_child(1), new_func);
}

void interpret_list(Node *node, function *func)
{
    if (node->get_type() != NodeType::LIST_NODE)
    {
        interpretation_error("List doesn't start with LIST Node", node, func);
    }

    // TODO: add support for nested lists

    for (int i = 0; i < (int)node->get_children().size(); i++)
    {
        interpret_expr(node->get_child(i), func);
        WRITE_BYTE(OpCode::OP_VECTOR_PUSH, func);    // Insert the value into the vector
        WRITE_BYTE(variable_names.size() - 1, func); // Index of the vector in the variables map
    }
}

void interpret_struct_assign(Node *node, function *func, std::string struct_name)
{
    if (node->get_type() != NodeType::ASSIGN_NODE)
    {
        interpretation_error("Struct assignment doesn't start with ASSIGN Node", node, func);
    }

    Node *var = node->get_child(0);
    std::string var_name = var->get_value();

    Node *value = node->get_child(1);

    if (var->get_type() != NodeType::VAR_NODE)
    {
        interpretation_error("Struct assignment doesn't have a VAR Node as the first child", node, func);
    }

    // TODO: add support for nested structs and lists
    // don't support lists in structs right now becasue of the OP_VECTOR_CREATE opcode
    // it just stores the vector in the variables map, so it can't be used in a struct
    // IDEA: make the OP_STORE_VAR opcode store any type of value in the variables map
    // make it take another argument that is encoded in the bytecode that tells it what type of value it is
    switch (value->get_type())
    {
    case NodeType::EXPR_NODE:
    {
        interpret_expr(value, func);

        WRITE_BYTE(OpCode::OP_UPDATE_STRUCT_ELEMENT, func); // Update the value in the struct
        WRITE_BYTE(get_variable_index(struct_name), func);  // index of the struct in the variables names array
        WRITE_VAR_NAME_IF_NOT_EXISTS(var_name);
        WRITE_BYTE(get_variable_index(var_name), func); // index of the struct element in the struct still in the variables names array
    }
    break;
    default:
        interpretation_error("Invalid child type for ASSIGN Node", node, func);
        break;
    }
}

void interpret_assign(Node *node, function *func)
{
    if (node->get_type() != NodeType::ASSIGN_NODE)
    {
        interpretation_error("Assign doesn't start with ASSIGN Node", node, func);
    }

    Node *var = node->get_child(0);
    std::string var_name = var->get_value();
    Node *value = node->get_child(1);

    if (var->get_type() != NodeType::VAR_NODE)
    {
        interpretation_error("Assign doesn't have a VAR Node as the first child", node, func);
    }

    switch (value->get_type())
    {
    case NodeType::EXPR_NODE:
    {
        interpret_expr(value, func);

        WRITE_BYTE(OpCode::OP_STORE_VAR, func); // takes the value from the stack and stores it in the variables map
        WRITE_VAR_NAME_IF_NOT_EXISTS(var_name);
        WRITE_BYTE(get_variable_index(var_name), func);
    }
    break;
    case NodeType::FUNCTION_NODE:
    {
        // have to do this first incase function calls itself
        WRITE_VAR_NAME_IF_NOT_EXISTS(var_name);

        interpret_function(value, func, std::string(var_name));

        WRITE_BYTE(OpCode::OP_STORE_VAR, func); // takes the value from the stack and stores it in the variables map
        WRITE_BYTE(get_variable_index(var_name), func);
    }
    break;
    case NodeType::LIST_NODE:
    {
        WRITE_BYTE(OpCode::OP_CREATE_VECTOR, func); // Create an empty vector and push it to the stack
        WRITE_VAR_NAME_IF_NOT_EXISTS(var_name);
        // Store var
        WRITE_BYTE(OpCode::OP_STORE_VAR, func); // takes the value from the stack and stores it in the variables map
        WRITE_BYTE(get_variable_index(var_name), func);

        interpret_list(value, func);
    }
    break;
    case NodeType::NULL_NODE:
    {
        WRITE_BYTE(OpCode::OP_LOAD, func);
        WRITE_BYTE(constants.size(), func);
        WRITE_VALUE({Value_Type::NULL_VALUE, nullptr});
        WRITE_BYTE(OpCode::OP_STORE_VAR, func); // takes the value from the stack and stores it in the variables map
        WRITE_VAR_NAME_IF_NOT_EXISTS(var_name);
        WRITE_BYTE(get_variable_index(var_name), func);
    }
    break;
    case NodeType::STRUCT_NODE:
    {
        // Create the struct
        WRITE_BYTE(OpCode::OP_CREATE_STRUCT, func); // Create an empty struct
        WRITE_VAR_NAME_IF_NOT_EXISTS(var_name);

        WRITE_BYTE(OpCode::OP_STORE_VAR, func); // takes the value from the stack and stores it in the variables map
        WRITE_BYTE(get_variable_index(var_name), func);

        // Assign the values to the struct
        Node *list = value->get_child(0);
        for (int i = 0; i < (int)list->get_children().size(); i++)
        {
            // assignment nodes
            Node *assign = list->get_child(i);
            if (assign->get_type() != NodeType::ASSIGN_NODE)
            {
                interpretation_error("Struct assignment doesn't start with ASSIGN Node", assign, func);
            }

            interpret_struct_assign(assign, func, var_name);
        }
    }
    break;
    default:
        interpretation_error("Invalid child type for ASSIGN Node", node, func);
        break;
    }
}

void interpret_update(Node *node, function *func)
{
    if (node->get_type() != NodeType::UPDATE_NODE)
    {
        interpretation_error("Update doesn't start with UPDATE Node", node, func);
    }

    Node *variable = node->get_child(0);

    if (variable->get_type() != NodeType::VAR_NODE)
    {
        interpretation_error("Update doesn't have a VAR Node as the first child", node, func);
    }

    std::vector<Node *> node_children = node->get_children();

    if (node_children.size() != 2)
    { 
        interpretation_error("Invalid number of children for UPDATE Node", node, func);
    }
    std::vector<Node *> variable_children = variable->get_children();

    if (variable_children.size() == 0)
    {                                           // Normal update
        interpret_expr(node_children[1], func); // Expression to update variable with

        WRITE_BYTE(OpCode::OP_UPDATE_VAR, func); // takes the value from the stack and updates the value in the variables map
        int index = get_variable_index(variable->get_value());
        if (index == -1)
        {
            interpretation_error("Trying to update a variable that hasn't been defined", node, func);
        }
        WRITE_BYTE(index, func);
    }
    else
    {                                           // Struct or vector update
        // std::cout << "Object update" << std::endl;
        std::vector<Node *> variable_children = variable->get_children();

        WRITE_BYTE(OpCode::OP_LOAD_VAR, func);
        int index = get_variable_index(variable->get_value());
        if (index == -1)
        {
            interpretation_error("Trying to access a variable that hasn't been defined", node, func);
        }
        WRITE_BYTE(index, func);

        int num_accesses = variable_children.size();
        for(int i = 0; i < num_accesses - 1; i++){ // reverse loop to keep the order of the accesses (want to access the elements closer to the varibale first)
            interpret_expr(variable_children[i], func);
            WRITE_BYTE(OpCode::OP_ACCESS_FOR_UPDATE, func);
        }

        interpret_expr(variable_children[num_accesses - 1], func); // Index to update
        interpret_expr(node_children[1], func); // Expression to update variable with

        for(int i = 0; i < num_accesses; i++){
            WRITE_BYTE(OpCode::OP_UPDATE_STACK_ELEMENT, func);
        }

        WRITE_BYTE(OpCode::OP_UPDATE_VAR, func); // takes the value from the stack and updates the value in the variables map
        WRITE_BYTE(index, func);
    }
}

void interpret_print(Node *node, function *func)
{
    if (node->get_type() != NodeType::PRINT_NODE)
    {
        interpretation_error("Print doesn't start with PRINT Node", node, func);
    }

    interpret_expr(node->get_child(0), func); // Expression to print

    WRITE_BYTE(OpCode::OP_PRINT, func);
}

void interpret_for(Node *node, function *func)
{
    if (node->get_type() != NodeType::FOR_NODE)
    {
        interpretation_error("For doesn't start with FOR Node", node, func);
    }

    WRITE_BYTE(OpCode::OP_INC_SCOPE, func); // Increase the scope for the for loop

    std::vector<Node *> children = node->get_children();
    std::vector<NodeType> child_types;
    // print the children
    for (int i = 0; i < (int)children.size(); i++)
    {
        child_types.push_back(children[i]->get_type());
    }

    // find the index of the first assign node
    int assign_index = -1;
    for (int i = 0; i < (int)child_types.size(); i++)
    {
        if (child_types[i] == NodeType::ASSIGN_NODE)
        {
            assign_index = i;
            break;
        }
    }
    if (assign_index != -1)
    {
        interpret_assign(node->get_child(assign_index), func); // Initialize the for loop
    }

    // get the index to jump back to
    int start_byte = func->count - 1;

    // find if there is an expression node
    int expr_index = -1;
    for (int i = 0; i < (int)child_types.size(); i++)
    {
        if (child_types[i] == NodeType::EXPR_NODE)
        {
            expr_index = i;
            break;
        }
    }
    if (expr_index != -1)
    {
        interpret_expr(node->get_child(expr_index), func); // Interpret the condition for the for loop

        // jump if the condition is false
        WRITE_BYTE(OpCode::OP_JUMP_IF_FALSE, func);

        // get the index to jump to the end of the for loop
        WRITE_BYTE(0, func); // Placeholder for the end of for loop jump
    }
    int jump_to_end_byte = func->count - 1;

    // find the index of the statement list node
    int stmt_list_index = -1;
    for (int i = 0; i < (int)child_types.size(); i++)
    {
        if (child_types[i] == NodeType::STMT_LIST_NODE)
        {
            stmt_list_index = i;
            break;
        }
    }
    if (stmt_list_index != -1)
    {
        interpret_stmt_list(node->get_child(stmt_list_index), func); // Interpret the body of the for loop
    }

    // find the index of the update node
    int update_index = -1;
    for (int i = 0; i < (int)child_types.size(); i++)
    {
        if (child_types[i] == NodeType::UPDATE_NODE)
        {
            update_index = i;
            break;
        }
    }
    int update_byte = func->count - 1;
    if (update_index != -1)
    {
        interpret_update(node->get_child(update_index), func); // Update the for loop
    }

    // jump back to the condition
    WRITE_BYTE(OpCode::OP_JUMP, func);
    WRITE_BYTE(start_byte, func);

    // find all flagged bytes from start_byte to the end of the for loop
    for (int i = start_byte; i < func->count; i++)
    {
        if (func->flags.size() == 0)
        {
            break;
        }
        for (int j = 0; j < (int)func->flags.size(); j++)
        {
            if (std::get<0>(func->flags[j]) == i)
            {
                if (std::get<1>(func->flags[j]) == "continue")
                {
                    CHANGE_BYTE(i, update_byte, func); // jump to the update part of the for loop
                    func->flags.erase(func->flags.begin() + j);
                }
                else if (std::get<1>(func->flags[j]) == "break")
                {
                    CHANGE_BYTE(i, func->count - 1, func); // jump to the end of the for loop
                    func->flags.erase(func->flags.begin() + j);
                }
                break;
            }
        }
    }

    // jump to the end of the for loop
    if (expr_index != -1)
    {
        CHANGE_BYTE(jump_to_end_byte, func->count - 1, func);
    }

    WRITE_BYTE(OpCode::OP_DEC_SCOPE, func); // Decrease the scope for the for loop
}

void interpret_stmt(Node *node, function *func)
{
    if (node->get_type() == NodeType::STMT_NODE)
    {
        Node *child = node->get_child(0);
        switch (child->get_type())
        {
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
        case NodeType::CONTINUE_NODE:
            // add a jump and flag the bytecode
            WRITE_BYTE(OpCode::OP_JUMP, func);
            WRITE_BYTE(0, func); // Placeholder for the jump index
            FLAG_BYTE(func->count - 1, "continue", func);
            break;
        case NodeType::BREAK_NODE:
            // add a jump and flag the bytecode
            WRITE_BYTE(OpCode::OP_JUMP, func);
            WRITE_BYTE(0, func); // Placeholder for the jump index
            FLAG_BYTE(func->count - 1, "break", func);
            break;
        case NodeType::STD_LIB_CALL_NODE:
            interpret_std_lib_call(child, func);
            break;
        default:
            interpretation_error("Invalid statement type", node, func);
            break;
        }
    }
    else
    {
        interpretation_error("Statement doesn't start with STMT Node", node, func);
    }
}

void interpret_stmt_list(Node *node, function *func)
{
    if (node->get_children().size() == 0)
    {
        return;
    }

    if (node->get_type() != NodeType::STMT_LIST_NODE)
    {
        interpretation_error("Statement List doesn't start with STMT_LIST Node", node, func);
    }

    interpret_stmt(node->get_child(0), func);
    if (node->get_children().size() == 2)
    {
        interpret_stmt_list(node->get_child(1), func);
    }
}

void interpret(Node *node, function *func)
{
    if (node->get_type() != NodeType::STMT_LIST_NODE)
    {
        interpretation_error("Program doesn't start with STMT_LIST Node", node, func);
    }

    interpret_stmt_list(node, func);
}

function *generate_bytecode(Node *ast, std::string name)
{
    function *func = create_function(1000);

    interpret(ast, func);

    write_cl_exe(name, "./", func, variable_names, constants);

    return func;
}

// -------------------------------------------------------------------

#endif // BYTECODE_GENERATOR_HPP