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
    OP_LOAD_VAR, // push a value from the variables map to the stack
    // Control flow
    OP_RETURN,
    OP_JUMP,
    OP_JUMP_IF_FALSE
};


// Data Structures ---------------------------------------------------
struct bytecode {
    int8_t* code; // Bytecode array
    int count;
    int capacity;
};

bytecode bc; // Statically allocated because only one bytecode array is needed

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
void display_bytecode(){
    for(int i = 0; i < bc.count; i++){
        std::cout << i << ": ";
        switch(bc.code[i]){
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
            case OpCode::OP_LOAD:
                std::cout << "OP_LOAD";
                std::cout << "          ";
                std::cout << "Index: " << (int)bc.code[++i];
                std::cout << "          ";
                std::cout << "Value: " << vals.values[(int)bc.code[i]] << std::endl;
                break;
            case OpCode::OP_RETURN:
                std::cout << "OP_RETURN" << std::endl;
                break;
            case OpCode::OP_JUMP:
                std::cout << "OP_JUMP    " << "Offset: " << (int)bc.code[++i] << std::endl;
                break;
            case OpCode::OP_JUMP_IF_FALSE:
                std::cout << "OP_JUMP_IF_FALSE    " << "Offset: " << (int)bc.code[++i] << std::endl;
                break;
            case OpCode::OP_STORE_VAR:
                std::cout << "OP_STORE_VAR";
                std::cout << "          ";
                std::cout << "Index: " << (int)bc.code[++i];
                std::cout << "          ";
                std::cout << "Name: " << variable_names.names[(int)bc.code[i]] << std::endl;
                break;
            case OpCode::OP_LOAD_VAR:
                std::cout << "OP_LOAD_VAR";
                std::cout << "          ";
                std::cout << "Index: " << (int)bc.code[++i];
                std::cout << "          ";
                std::cout << "Name: " << variable_names.names[(int)bc.code[i]] << std::endl;
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
#define WRITE_BYTE(byte) bc.code[bc.count++] = byte
#define WRITE_VALUE(double) vals.values[vals.count++] = double

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
// -------------------------------------------------------------------

// Interpretation ----------------------------------------------------
void interpret(Node* node);
void interpret_stmt_list(Node* node);
void interpret_stmt(Node* node);
void interpret_if(Node* node);
void interpret_return(Node* node);
void interpret_expr(Node* node);
void interpret_op(Node* node);

void interpretation_error(std::string message, Node* node){
    std::cout << message << std::endl;
    node->print();
    exit(1);
}

void interpret_op(Node* node){
    if(node->get_type() == NodeType::OP){
        Node* l_child = node->get_child(0);
        Node* r_child = node->get_child(1);

        if(l_child->get_type() == NodeType::OP){
            interpret_op(l_child);
        }
        else if(l_child->get_type() == NodeType::NUM){
            vals.values[vals.count++] = std::stod(l_child->get_value());
            //bc.code[bc.count++] = OpCode::OP_LOAD;
            //bc.code[bc.count++] = vals.count - 1;
            WRITE_BYTE(OpCode::OP_LOAD);
            WRITE_BYTE(vals.count - 1);
        }
        else if(l_child->get_type() == NodeType::VAR){
            WRITE_BYTE(OpCode::OP_LOAD_VAR);
            if(get_variable_index(l_child->get_value()) == -1){
                interpretation_error("Variable not found", node);
            }
            WRITE_BYTE(get_variable_index(l_child->get_value()));
        }
        else{
            interpretation_error("Invalid child type for OP Node", node);
        }

        if(r_child->get_type() == NodeType::OP){
            interpret_op(r_child);
        }
        else if(r_child->get_type() == NodeType::NUM){
            vals.values[vals.count++] = std::stod(r_child->get_value());
            // bc.code[bc.count++] = OpCode::OP_LOAD;
            // bc.code[bc.count++] = vals.count - 1;
            WRITE_BYTE(OpCode::OP_LOAD);
            WRITE_BYTE(vals.count - 1);
        }
        else if(r_child->get_type() == NodeType::VAR){
            WRITE_BYTE(OpCode::OP_LOAD_VAR);
            if(get_variable_index(r_child->get_value()) == -1){
                interpretation_error("Variable not found", node);
            }
            WRITE_BYTE(get_variable_index(r_child->get_value()));
        }
        else{
            interpretation_error("Invalid child type for OP Node", node);
        }
        
        switch(node->get_value()[0]){
            case '+':
                //bc.code[bc.count++] = OpCode::OP_ADD;
                WRITE_BYTE(OpCode::OP_ADD);
                break;
            case '-':
                //bc.code[bc.count++] = OpCode::OP_SUB;
                WRITE_BYTE(OpCode::OP_SUB);
                break;
            case '*':
                //bc.code[bc.count++] = OpCode::OP_MUL;
                WRITE_BYTE(OpCode::OP_MUL);
                break;
            case '/':
                //bc.code[bc.count++] = OpCode::OP_DIV;
                WRITE_BYTE(OpCode::OP_DIV);
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

void interpret_expr(Node* node){
    if(node->get_type() == NodeType::EXPR){
        if(node->get_child(0)->get_type() == NodeType::OP){
            interpret_op(node->get_child(0));
        }
        else if(node->get_child(0)->get_type() == NodeType::NUM){
            vals.values[vals.count++] = std::stod(node->get_child(0)->get_value());
            WRITE_BYTE(OpCode::OP_LOAD);
            WRITE_BYTE(vals.count - 1);
        }
        else if(node->get_child(0)->get_type() == NodeType::VAR){
            WRITE_BYTE(OpCode::OP_LOAD_VAR);
            if(get_variable_index(node->get_child(0)->get_value()) == -1){
                interpretation_error("Variable not found", node);
            }
            WRITE_BYTE(get_variable_index(node->get_child(0)->get_value()));
        }
        else{
            interpretation_error("Expression doesn't start with OP Node", node);
        }
    }
    else{
        interpretation_error("Expression doesn't start with EXPR Node", node);
    }
}

void interpret_return(Node* node){
    if(node->get_type() != NodeType::RETURN){
        interpretation_error("Return doesn't start with RETURN Node", node);
    }

    interpret_expr(node->get_child(0));

    //bc.code[bc.count++] = OpCode::OP_RETURN;
    WRITE_BYTE(OpCode::OP_RETURN);
}

void interpret_if(Node* node){
    if(node->get_type() != NodeType::IF){
        interpretation_error("If doesn't start with IF Node", node);
    }

    interpret_expr(node->get_child(0));
    WRITE_BYTE(OpCode::OP_JUMP_IF_FALSE);
    int jump_index = bc.count;
    WRITE_BYTE(0); // Placeholder for the jump index

    interpret_stmt_list(node->get_child(1));

    int jump_offset = bc.count - jump_index;
    bc.code[jump_index] = jump_offset;

    if(node->get_children().size() == 3){
        bc.code[jump_index] = jump_offset + 2; // Plus 2 because of the jump instruction

        WRITE_BYTE(OpCode::OP_JUMP);
        int jump_index = bc.count;
        WRITE_BYTE(0); // Placeholder for the jump index

        interpret_stmt_list(node->get_child(2));

        int jump_offset = bc.count - jump_index;
        bc.code[jump_index] = jump_offset;
    }
}

void interpret_assign(Node* node){
    if(node->get_type() != NodeType::ASSIGN){
        interpretation_error("Assign doesn't start with ASSIGN Node", node);
    }

    if(node->get_child(0)->get_type() != NodeType::VAR){
        interpretation_error("Assign doesn't have a VAR Node as the first child", node);
    }

    interpret_expr(node->get_child(1));

    //std::cout << "Variable: " << node->get_child(0)->get_value() << std::endl;

    WRITE_BYTE(OpCode::OP_STORE_VAR); // takes the value from the stack and stores it in the variables map
    if(get_variable_index(node->get_child(0)->get_value()) == -1){
        WRITE_VAR_NAME(node->get_child(0)->get_value());
    }
    WRITE_BYTE(get_variable_index(node->get_child(0)->get_value()));
}

void interpret_stmt(Node* node){
    if(node->get_type() == NodeType::STMT){
        Node* child = node->get_child(0);
        switch(child->get_type()){
            case NodeType::EXPR:
                interpret_expr(child);
                break;
            case NodeType::RETURN:
                interpret_return(child);
                break;
            case NodeType::IF:
                interpret_if(child);
                break;
            case NodeType::ASSIGN:
                interpret_assign(child);
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

void interpret_stmt_list(Node* node){
    if(node->get_children().size() == 0){
        return;
    }

    if(node->get_type() != NodeType::STMT_LIST){
        interpretation_error("Statement List doesn't start with STMT_LIST Node", node);
    }

    interpret_stmt(node->get_child(0));
    if(node->get_children().size() == 2){
        interpret_stmt_list(node->get_child(1));
    }
}

void interpret(Node* node){
    if(node->get_type() != NodeType::PROGRAM){
        interpretation_error("Program doesn't start with PROGRAM Node", node);
    }
    
    interpret_stmt_list(node->get_child(0));
}

void generate_bytecode(Node* ast) {
    bc.code = new int8_t[1000]; // Allocate 1000 bytes for the bytecode
    bc.count = 0;
    bc.capacity = 1000;

    vals.values = new double[1000]; // Allocate 1000 doubles for the values
    vals.count = 0;
    vals.capacity = 1000;

    variable_names.names = new std::string[1000]; // Allocate 1000 strings for the variable names
    variable_names.count = 0;
    variable_names.capacity = 1000;

    interpret(ast);

    // // add return statement to the end of the bytecode
    // bc.code[bc.count++] = OpCode::OP_RETURN;
}

// -------------------------------------------------------------------

#endif // BYTECODE_GENERATOR_HPP