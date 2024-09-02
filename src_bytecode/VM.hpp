#ifndef VM_HPP
#define VM_HPP

#include <vector>
#include <string>

#include "Value.hpp"


struct function_frame
{
    function *func;
    int current_scope;

    CODE_SIZE *ip; // Pointer to the current instruction
    int end_of_function;
    int current_instruction;

    std::vector<std::map<std::string, Value>> variables; // Variables in the current function
};

struct VM
{
    Value *stack;
    int stack_count;
    int stack_capacity;

    std::vector<Value> constants;
    std::vector<std::string> variable_names;

    std::vector<function_frame *> function_frames;

    bool jit;
};

VM vm; // Statically allocated because only one VM is needed

void vm_error(const std::string &message)
{
    std::cout << "ERROR: " << message << std::endl;
    exit(1);
}

// Helper functions ---------------------------------------------------

function_frame *get_current_function_frame(VM* vm)
{
    return vm->function_frames.back();
}

function_frame *get_function_frame(VM* vm, int index)
{
    return vm->function_frames[index];
}

function_frame *create_function_frame(function *func)
{
    function_frame *frame = new function_frame();
    frame->func = func;
    frame->current_scope = 0;
    frame->ip = func->code;
    frame->end_of_function = func->count;
    frame->current_instruction = 0;
    frame->variables.push_back(std::map<std::string, Value>());

    return frame;
}


bool increase_ip(VM* vm, int offset)
{
    function_frame *frame = get_current_function_frame(vm);

    if (frame->ip + offset >= frame->func->code + frame->end_of_function)
    { // Probably slow
        // if in the main function just return
        if (vm->function_frames.size() == 1)
        {
            return false;
        }
        vm_error("End of function reached without return");
    }

    frame->ip += offset;

    return true;
}

bool goto_ip(VM* vm, int index)
{
    function_frame *frame = get_current_function_frame(vm);

    if (index >= frame->func->count || index < 0)
    {
        return false;
    }

    frame->ip = &frame->func->code[index];

    return true;
}

Value get_vm_constant(VM* vm, int index)
{
    return vm->constants[index];
}

// get ip from the current function frame
CODE_SIZE *get_ip(VM* vm)
{
    return get_current_function_frame(vm)->ip;
}

// Stack operations -------------------------------------------------
void push(VM* vm, Value value)
{
    vm->stack[vm->stack_count++] = value;
}

Value pop(VM* vm)
{
    return vm->stack[--vm->stack_count];
}

Value top(VM* vm)
{
    return vm->stack[vm->stack_count - 1];
}

void print_stack(VM* vm)
{
    for (int i = 0; i < vm->stack_count; i++)
    {
        print_value(vm->stack[i]);
        std::cout << " ";
    }
    std::cout << std::endl;
}

// -------------------------------------------------------------------

// Map operations -----------------------------------------------------

// Creates a new variable in the current scope
void set_variable(VM* vm, const std::string &name, Value value)
{
    function_frame *frame = get_current_function_frame(vm);
    frame->variables[frame->current_scope][name] = value;
}

// Updates a variable in the current function frame, does not look in the parent frames
// Looks for the variable in the closest scope, so climbs out of ifs, loops, etc.
void update_variable(VM* vm, const std::string &name, Value value)
{
    function_frame *frame = get_current_function_frame(vm);
    for (int i = frame->current_scope; i >= 0; i--)
    {
        if (frame->variables[i].find(name) != frame->variables[i].end())
        {
            frame->variables[i][name] = value;
            return;
        }
    }
    vm_error("update variable: Variable " + name + " not found");
}

// Gets a variable in the current function frame, does not look in the parent frames
// Looks for the variable in the closest scope, so climbs out of ifs, loops, etc.
Value get_variable(VM* vm, const std::string &name)
{
    function_frame *frame = get_current_function_frame(vm);
    for (int i = frame->current_scope; i >= 0; i--)
    {
        if (frame->variables[i].find(name) != frame->variables[i].end())
        {
            return frame->variables[i][name];
        }
    }
    vm_error("get_variable: Variable " + name + " not found");
    return Value(); // To avoid warning, but this line will never be reached because of vm_error
}

Value get_function_variable(VM* vm, const std::string &name)
{
    for(int j = 0; j < (int)vm->function_frames.size(); j++){
        function_frame *frame = vm->function_frames[j];
        for (int i = frame->current_scope; i >= 0; i--)
        {
            if (frame->variables[i].find(name) != frame->variables[i].end())
            {
                return frame->variables[i][name];
            }
        }
    }
    vm_error("get_function_variable: Variable " + name + " not found");
    return Value(); // To avoid warning, but this line will never be reached because of vm_error
}
// -------------------------------------------------------------------


#endif // VM_HPP