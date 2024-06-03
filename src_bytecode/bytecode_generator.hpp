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

// -------------------------------------------------------------------

// Helper functions --------------------------------------------------
#define WRITE_BYTE(byte) bc.code[bc.count++] = byte
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
            // bc.code[bc.count++] = OpCode::OP_LOAD;
            // bc.code[bc.count++] = vals.count - 1;
            WRITE_BYTE(OpCode::OP_LOAD);
            WRITE_BYTE(vals.count - 1);
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

    //interpret_expr(node->get_child(0));

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

    interpret(ast);

    // // add return statement to the end of the bytecode
    // bc.code[bc.count++] = OpCode::OP_RETURN;
}

// -------------------------------------------------------------------

#endif // BYTECODE_GENERATOR_HPP