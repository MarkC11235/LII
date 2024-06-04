#ifndef VIRUTAL_MACHINE_HPP
#define VIRUTAL_MACHINE_HPP

#include <cstdint> // int8_t
#include "bytecode_generator.hpp"

// Data Structures ---------------------------------------------------
struct VM{
    double* stack;
    int stack_count;
    int stack_capacity;

    std::map<std::string, int> variables;

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

// Map operations -----------------------------------------------------
void set_variable(const std::string& name, double value){
    vm.variables[name] = value;
}

double get_variable(const std::string& name){
    return vm.variables[name];
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
            case OpCode::OP_JUMP:
                vm.ip += vm.ip[1];
                break;
            case OpCode::OP_JUMP_IF_FALSE:
                if(pop() == 0){
                    vm.ip += vm.ip[1];
                } else {
                    vm.ip++;
                }
                break;
            case OpCode::OP_STORE_VAR:
                set_variable(variable_names.names[vm.ip[1]], pop());
                vm.ip++;
                break;
            case OpCode::OP_LOAD_VAR:
                push(get_variable(variable_names.names[vm.ip[1]]));
                vm.ip++;
                break;
            default:
                std::cout << "ERROR: Unknown opcode" << std::endl;
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