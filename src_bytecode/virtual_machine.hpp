#ifndef VIRUTAL_MACHINE_HPP
#define VIRUTAL_MACHINE_HPP

#include <cstdint> // int8_t
#include "bytecode_generator.hpp"

// Data Structures ---------------------------------------------------
struct VM{
    double* stack;
    int stack_count;
    int stack_capacity;

    int8_t* ip; // Pointer to the current instruction
};

VM vm; // Statically allocated because only one VM is needed

// -------------------------------------------------------------------

// Initializes the virtual machine ----------------------------------
void init_vm(){
    vm.stack = new double[256];
    vm.stack_count = 0;
    vm.stack_capacity = 256;

    vm.ip = bc.code;
}

// -------------------------------------------------------------------

// Stack operations -------------------------------------------------
void push(double value){
    vm.stack[vm.stack_count++] = value;
}

double pop(){
    return vm.stack[--vm.stack_count];
}

// -------------------------------------------------------------------

// Runs the virtual machine ------------------------------------------
void run_vm(bool verbose = false){
    if(verbose){
        std::cout << "Running VM" << std::endl;
    }

    while(true){
        if(verbose){
            std::cout << "IP: " << (int)(vm.ip - bc.code) << std::endl;
        }

        switch(*vm.ip){
            case OpCode::OP_ADD:
                push(pop() + pop());
                break;
            case OpCode::OP_SUB:
                push(pop() - pop());
                break;
            case OpCode::OP_MUL:
                push(pop() * pop());
                break;
            case OpCode::OP_DIV:
            { // Braces are needed because of the declaration of dividend and divisor
                double dividend = pop();
                double divisor = pop();
                if(divisor == 0){
                    std::cout << "ERROR: Division by zero" << std::endl;
                    return;
                }
                push(dividend / divisor);
                break;
            }
            case OpCode::OP_LOAD:
                push(vals.values[vm.ip[1]]);
                vm.ip++;
                break;
            case OpCode::OP_RETURN:
                std::cout << pop() << std::endl; //Temporarily print the result
                return;
        }
        vm.ip++;
    }
}

// -------------------------------------------------------------------

// Starts the interpretation process ---------------------------------
void interpret_bytecode(bool verbose = false){
    init_vm();
    run_vm(verbose);
}

// -------------------------------------------------------------------

#endif // VIRUTAL_MACHINE_HPP