#ifndef OPCODES_HPP
#define OPCODES_HPP


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
                Index of the variable name in the variable names array is the next byte
    */
    OP_STORE_VAR, 
    /*
    * OP_UPDATE_VAR: Update a value in the variables map with a value from the stack
                Index of the variable name in the variable names array is the next byte
                var type is the next byte -> 
                    0: normal variable (number, bool, string, function, null)
                    1: vector
                    2: struct
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
    * OP_VECTOR_PUSH: Push a value onto the vector
                Index of the vector in the variable names array is the next byte
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

    // Structs

    /*
    * OP_CREATE_STRUCT: Make an empty struct and push it onto the stack
    */
    OP_CREATE_STRUCT, 
    /*
    * OP_LOAD_STRUCT_ELEMENT: Load a value from the struct and push it onto the stack
                Index of the struct in the variable names array is the next byte
                Name of the element in the struct is the next byte
    */
    OP_LOAD_STRUCT_ELEMENT, 
    /*
    * OP_UPDATE_STRUCT_ELEMENT: Update a value in the struct
                Index of the struct in the variable names array is the next byte
                Name of the element in the struct is the next byte
                The value to update is on the stack
    */
    OP_UPDATE_STRUCT_ELEMENT, 

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

std::string opcode_to_string(CODE_SIZE op){
    switch(op){
        case OpCode::OP_ADD:
            return "OP_ADD";
        case OpCode::OP_SUB:
            return "OP_SUB";
        case OpCode::OP_U_SUB:
            return "OP_U_SUB";
        case OpCode::OP_MUL:
            return "OP_MUL";
        case OpCode::OP_DIV:
            return "OP_DIV";
        case OpCode::OP_MOD:
            return "OP_MOD";
        case OpCode::OP_AND:
            return "OP_AND";
        case OpCode::OP_OR:
            return "OP_OR";
        case OpCode::OP_NOT:
            return "OP_NOT";
        case OpCode::OP_EQ:
            return "OP_EQ";
        case OpCode::OP_NEQ:
            return "OP_NEQ";
        case OpCode::OP_GT:
            return "OP_GT";
        case OpCode::OP_LT:
            return "OP_LT";
        case OpCode::OP_GTEQ:
            return "OP_GTEQ";
        case OpCode::OP_LTEQ:
            return "OP_LTEQ";
        case OpCode::OP_LOAD:
            return "OP_LOAD";
        case OpCode::OP_STORE_VAR:
            return "OP_STORE_VAR";
        case OpCode::OP_UPDATE_VAR:
            return "OP_UPDATE_VAR";
        case OpCode::OP_LOAD_VAR:
            return "OP_LOAD_VAR";
        case OpCode::OP_LOAD_FUNCTION_VAR:
            return "OP_LOAD_FUNCTION_VAR";
        case OpCode::OP_CREATE_VECTOR:
            return "OP_CREATE_VECTOR";
        case OpCode::OP_VECTOR_PUSH:
            return "OP_VECTOR_PUSH";
        case OpCode::OP_LOAD_VECTOR_ELEMENT:
            return "OP_LOAD_VECTOR_ELEMENT";
        case OpCode::OP_UPDATE_VECTOR_ELEMENT:
            return "OP_UPDATE_VECTOR_ELEMENT";
        case OpCode::OP_CREATE_STRUCT:
            return "OP_CREATE_STRUCT";
        case OpCode::OP_LOAD_STRUCT_ELEMENT:
            return "OP_LOAD_STRUCT_ELEMENT";
        case OpCode::OP_UPDATE_STRUCT_ELEMENT:
            return "OP_UPDATE_STRUCT_ELEMENT";
        case OpCode::OP_RETURN:
            return "OP_RETURN";
        case OpCode::OP_JUMP:
            return "OP_JUMP";
        case OpCode::OP_JUMP_IF_FALSE:
            return "OP_JUMP_IF_FALSE";
        case OpCode::OP_FUNCTION_CALL:
            return "OP_FUNCTION_CALL";
        case OpCode::OP_INC_SCOPE:
            return "OP_INC_SCOPE";
        case OpCode::OP_DEC_SCOPE:
            return "OP_DEC_SCOPE";
        case OpCode::OP_PRINT:
            return "OP_PRINT";
        case OpCode::OP_STD_LIB_CALL:
            return "OP_STD_LIB_CALL";
        default:
            return "INVALID OPCODE";    
    }
}

#endif // OPCODES_HPP