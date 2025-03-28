#ifndef OPCODES_HPP
#define OPCODES_HPP

#include "Function.hpp"

enum OpCode{
    // Arithmetic

    /*
    * OP_ADD: Add the top two values on the stack
                Pushes the result back onto the stack
    */
    OP_ADD,
    /*
    * OP_SUB: Subtract the top two values on the stack
                Pushes the result back onto the stack
    */
    OP_SUB,
    /*
    * OP_U_SUB: Unary subtraction, negate the top value on the stack
                Pushes the result back onto the stack
    */
    OP_U_SUB,
    /*
    * OP_MUL: Multiply the top two values on the stack
                Pushes the result back onto the stack
    */
    OP_MUL,
    /*
    * OP_DIV: Divide the top two values on the stack
                Pushes the result back onto the stack
                If the second value is 0, throws a runtime error
    */
    OP_DIV,
    /*
    * OP_MOD: Modulus of the top two values on the stack
                Pushes the result back onto the stack
                If the second value is 0, throws a runtime error
    */
    OP_MOD,
    /*
    * OP_EXP: Exponentiation of the top two values on the stack
                Pushes the result back onto the stack
    */
    OP_EXP,
    // Boolean
    /*
    * OP_AND: Logical AND of the top two values on the stack
                Pushes the result back onto the stack
    */
    OP_AND,
    /*
    * OP_OR: Logical OR of the top two values on the stack
                Pushes the result back onto the stack
    */
    OP_OR,
    /*
    * OP_NOT: Logical NOT of the top value on the stack
                Pushes the result back onto the stack
    */
    OP_NOT,

    //Comparison

    /*
    * OP_EQ: Check if the top two values on the stack are equal
                Pushes the result back onto the stack
                Will type coerce unless both values are numbers or both values are strings
    */
    OP_EQ,
    /*
    * OP_NEQ: Check if the top two values on the stack are not equal
                Pushes the result back onto the stack
                Will type coerce unless both values are numbers or both values are strings
    */
    OP_NEQ,
    /*
    * OP_GT: Check if the second value on the stack is greater than the top value on the stack
                Pushes the result back onto the stack
                Throws a runtime error if the values are not numbers
    */
    OP_GT,
    /*
    * OP_LT: Check if the second value on the stack is less than the top value on the stack
                Pushes the result back onto the stack
                Throws a runtime error if the values are not numbers
    */
    OP_LT,
    /*
    * OP_GTEQ: Check if the second value on the stack is greater than or equal to the top value on the stack
                Pushes the result back onto the stack
                Throws a runtime error if the values are not numbers
    */
    OP_GTEQ,
    /*
    * OP_LTEQ: Check if the second value on the stack is less than or equal to the top value on the stack
                Pushes the result back onto the stack
                Throws a runtime error if the values are not numbers
    */
    OP_LTEQ,
    // Variables

    /*
    * OP_LOAD: Push a value from the constants array to the stack
                Index of the constants array is the next byte
    */
    OP_LOAD,
    /*
    * OP_STORE_VAR: Store a value from the stack to the variables map
                Type of the assignment is the next byte -> 
                    0: let
                    1: const
                    2: global
                Index of the variable name in the variable names array is the next byte
    */
    OP_STORE_VAR, 
    /*
    * OP_UPDATE_VAR: Update a value in the variables map with a value from the stack
                Index of the variable name in the variable names array is the next byte
                var type is the next byte -> 
                    0: normal variable (number, bool, string, function, null)
                    1: vector
                    2: map
    */
    OP_UPDATE_VAR, 
    /*
    * OP_LOAD_VAR: Push a value from the variables map to the stack
                Index of the variable name in the variable names array is the next byte
    */
    OP_LOAD_VAR, // push a value from the variables map to the stack
    /*
    * OP_LOAD_FUNCTION_VAR: Looks up the function frames to find the function variable
                Index of the variable name in the variable names array is the next byte
    */
    OP_LOAD_FUNCTION_VAR, 

    // Arrays

    /*
    * OP_CREATE_VECTOR: Make an empty vector and push it onto the stack 
    */
    OP_CREATE_VECTOR, 
    /*
    * OP_VECTOR_PUSH: Push a value into the back of the vector
                    The value is on the stack
                    The vector is below the value on the stack
    */
    OP_VECTOR_PUSH, 
    /*
    * OP_LOAD_VECTOR_ELEMENT: Load a value from the vector
                Index of the vector in the variable names array is the next byte
                The index of the value in the vector is on the stack
    */
    OP_LOAD_VECTOR_ELEMENT,
    /*
    * OP_UPDATE_VECTOR_ELEMENT: Update a value in the vector
                Index of the vector in the variable names array is the next byte
                The index of the value in the vector is on the stack
                The value to update is on the stack
    */
    OP_UPDATE_VECTOR_ELEMENT,

    // maps

    // CUSTOM_TYPES
    /*
    * OP_DEFINE_OP_FOR_TYPE: Define a custom operation for a custom type
                The operation (str) is on the stack
                The type (str) is the next stack value
                The function is the next stack value
    */
    OP_DEFINE_OP_FOR_TYPE,
    /*
    * OP_CREATE_MAP: Make an empty map and push it onto the stack
    */
    OP_CREATE_MAP, 
    /*
    * OP_LOAD_MAP_ELEMENT: Load a value from the map and push it onto the stack
                Index of the map in the variable names array is the next byte
                Name of the element in the map is the next byte
    */
    OP_LOAD_MAP_ELEMENT, 
    /*
    * OP_UPDATE_MAP_ELEMENT: Update a value in the map
                Index of the map in the variable names array is the next byte
                Name of the element in the map is the next byte
                The value to update is on the stack
    */
    OP_UPDATE_MAP_ELEMENT, 

    /*
    * OP_ACCESS: Access a value in a map or vector
                Index is the top value on the stack
                The map or vector is below the index on the stack
    */
    OP_ACCESS,
    /*
    * OP_ACCESS_FOR_UPDATE: Access a value in a map or vector, then push the original value back onto the stack, then the index, then the value to update
                            Index is the top value on the stack
                            The map or vector is below the index on the stack
    */
    OP_ACCESS_FOR_UPDATE,
    /*
    * OP_UPDATE_STACK_ELEMENT: Update an element on the stack and push it back onto the stack
                                Value is the top value on the stack
                                Index is the next value on the stack
                                element to update is below the index on the stack
    */
    OP_UPDATE_STACK_ELEMENT,
    /*
    * OP_ACCESS_STACK_ELEMENT_QUEUE: pop an element on the stack, get the first key:value pair, remove them from the map, push map back on stack, push the key and value back onto the stack
                                     Element is the top value on the stack
    */
    OP_ACCESS_STACK_ELEMENT_QUEUE,
    /*
    * OP_NOT_EMPTY: Check if the top value on the stack is not empty and push the value back onto the stack
                    Pushes the result back onto the stack
    */
    OP_NOT_EMPTY,
    /*
    * OP_POP: Pop the top value off the stack
    */
    OP_POP,

    // Control flow

    /*
    * OP_RETURN: Return the top value on the stack
                Pops the value off the stack
    */
    OP_RETURN,
    /*
    * OP_JUMP: Jump to a different part of the code
                Offset is the next byte
    */
    OP_JUMP,
    /*
    * OP_JUMP_IF_FALSE: Jump to a different part of the code if the top value on the stack is false
                Offset is the next byte
    */
    OP_JUMP_IF_FALSE,
    /*
    * OP_FUNCTION_CALL: Call a function
                The function is the top value on the stack
                The arguments are below the function on the stack
    */
    OP_FUNCTION_CALL,

    //Scope

    /*
    * OP_INC_SCOPE: Increase the scope level by 1
    */ 
    OP_INC_SCOPE,
    /*
    * OP_DEC_SCOPE: Decrease the scope level by 1
    */
    OP_DEC_SCOPE,

    // Output
    /*
    * OP_PRINT: Print the top value on the stack
                Pops the value off the stack
    */
    OP_PRINT,

    // Stdlib

    /*
    * OP_STD_LIB_CALL: Call a function from the standard library
                Index of the function in the standard library functions array is the next byte
                The args are on the stack
    */
    OP_STD_LIB_CALL
};

std::string opcode_to_string(CODE_SIZE op);

#endif // OPCODES_HPP