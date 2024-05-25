#ifndef VIRUTAL_MACHINE_HPP
#define VIRUTAL_MACHINE_HPP

#include <cstdint> // int8_t
#include "bytecode_generator.hpp"

struct VM{
    double* stack;
    int stack_count;
    int stack_capacity;

    int8_t* ip;
};

VM vm;

void init_vm(){
    vm.stack = new double[256];
    vm.stack_count = 0;
    vm.stack_capacity = 256;

    vm.ip = bc.code;
}

void push(double value){
    vm.stack[vm.stack_count++] = value;
}

double pop(){
    return vm.stack[--vm.stack_count];
}

void run_vm(){
    std::cout << "Running VM" << std::endl;
    while(true){
        std::cout << "IP: " << (int)(vm.ip - bc.code) << std::endl;
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
                push(pop() / pop());
                break;
            case OpCode::OP_LOAD:
                push(vals.values[vm.ip[1]]);
                vm.ip++;
                break;
            case OpCode::OP_RETURN:
                std::cout << "Result: " << pop() << std::endl; //Temporarily print the result
                return;
        }
        vm.ip++;
    }
}

void interpret_bytecode(){
    init_vm();
    run_vm();
}

#endif // VIRUTAL_MACHINE_HPP