#ifndef VM_HPP
#define VM_HPP

#include <vector>
#include <string>

#include "../helpers/Value.hpp"
#include "../helpers/variables.hpp"

/*
This holds information about the current function being executed
Is created when starting a program and for each function call
*/
struct function_frame
{
    function *func;
    int current_scope;

    CODE_SIZE *ip; // Pointer to the current instruction
    int end_of_function;
    int current_instruction;

    // the tuple holds the value and if the varibale is let or const (globals don't get put here) also the type of the variable (number, string, etc.)
    std::vector<std::map<std::string, Variable>> variables; // Variables in the current function
};

struct VM; // Forward declaration

typedef void (*JIT_FUNCTION)(VM* vm);

// #define CALLS_TO_JIT 1
#define JIT_OPTIMIZATION_LEVEL "-O2"

Value pop(VM* vm);
Value top(VM* vm);
void push(VM* vm, Value value);

/*
Created when starting the program
Holds all data about the program
*/
struct VM
{
    Value *stack;
    int stack_count;
    int stack_capacity;

    std::vector<Value> constants;
    std::vector<std::string> variable_names;

    std::vector<function_frame *> function_frames;

    // global variable map
    std::map<std::string, Variable> global_variables;

    // bool jit;
    // int calls_to_jit;
    // std::vector<JIT_FUNCTION> jit_functions;

    // CUSTOM_TYPES
    /*\
    // types
    vector< (name, default, custom_value) >

    // ops
    map{
        string (op)
        map{
            tuple(type, type) (for unary ops, the two strings in the tuple will be the same)
            Value (usually should be a function)
        }
    }
    */

    std::vector<std::tuple<std::string, Value, Value>> custom_types; // List of custom types
    std::map<std::string, std::map<std::tuple<std::string, std::string>, Value>> custom_ops; 
};

VM vm; // Statically allocated because only one VM is needed

// #include "jit.hpp"

void vm_error(const std::string &message)
{
    std::cout << "ERROR: " << message << std::endl;
    exit(1);
}


// Function frame operations ----------------------------------------
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
    frame->variables.push_back(std::map<std::string, Variable>());

    return frame;
}

/*
Pops a function from the stack and calls it
*/
void function_call(bool verbose = false){
    if (verbose)
    {
        std::cout << "Calling function: " << std::endl;
    }

    function* func = VALUE_AS_FUNCTION(pop(&vm));

    // TEMPORARY UNTIL I DECIDE TO REINTEGRE JIT

    // func->times_called++; // for jit compilation

    // if(vm.jit && func->times_called == vm.calls_to_jit){
    //     if(verbose){
    //         std::cout << "JIT compiling function: " << func->name << std::endl;
    //     }        
    //     jit_compile_function(&vm, func);
    // }

    // if(vm.jit && func->jit_index != -1){
    //     if(verbose){
    //         std::cout << "Calling JIT function: " << func->jit_index << std::endl;
    //     }
    //     vm.function_frames.push_back(create_function_frame(func));
    //     jit_run_function(&vm, func->jit_index);
    // }
    // else{
    //     vm.function_frames.push_back(create_function_frame(func));

    //     get_current_function_frame(&vm)->ip = func->code - 1; // -1 because the ip will be increased by 1
    // }

    vm.function_frames.push_back(create_function_frame(func));

    get_current_function_frame(&vm)->ip = func->code - 1; // -1 because the ip will be increased by 1
}
// -------------------------------------------------------------------

/*
Increments the instruction pointer by the given offset
Returns false if the end of the function is reached, occurs in main function because there is no return required in the main function
*/
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

/*
Sets the instruction pointer to the given index
Returns false if the index is out of bounds
*/
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

/*
Returns the current instruction of the current function frame
Throws an error if the instruction is out of bounds
*/
Value get_vm_constant(VM* vm, int index)
{
    if(index >= (int)vm->constants.size() || index < 0){
        vm_error("get_vm_constant: Constant index out of bounds");
    }
    return vm->constants[index];
}

/*
Gets the current instruction pointer of the current function frame
*/
CODE_SIZE *get_ip(VM* vm)
{
    return get_current_function_frame(vm)->ip;
}


// Stack operations -------------------------------------------------
/*
Pushes a value to the stack
*/
void push(VM* vm, Value value)
{
    vm->stack[vm->stack_count++] = value;
}

/*
Pops a value from the stack and returns it
Throws an error if the stack is empty
*/
Value pop(VM* vm)
{
    if(vm->stack_count == 0){
        vm_error("pop: Stack underflow");
    }
    return vm->stack[--vm->stack_count];
}

/*
Returns the top value of the stack without popping it
Throws an error if the stack is empty
*/
Value top(VM* vm)
{
    if(vm->stack_count <= 0){
        vm_error("top: Stack underflow");
    }
    return vm->stack[vm->stack_count - 1];
}

/*
Prints the stack from bottom to top
*/
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


// Variable operations ----------------------------------------------
/*
Creates a new variable in the current scope of the function frame
Will overwrite the variable if one with the same name already exists
*/
void set_variable(VM* vm, const std::string &name, Value value, int assignment_type, int variable_type)
{
    function_frame *frame = get_current_function_frame(vm);
    // frame->variables[frame->current_scope][name] = value;

    // convert variable type to string
    // std::string variable_type_string = "";
    // switch (variable_type)
    // {
    //     case 0:
    //         variable_type_string = "any";
    //         break;
    //     case 1:
    //         variable_type_string = "number";
    //         break;
    //     case 2:
    //         variable_type_string = "string";
    //         break;
    //     case 3:
    //         variable_type_string = "bool";
    //         break;
    //     case 4:
    //         variable_type_string = "null";
    //         break;
    //     case 5:
    //         variable_type_string = "vector";
    //         break;
    //     case 6: 
    //         variable_type_string = "map";
    //         break;
    //     case 7:
    //         variable_type_string = "func";
    //         break;
    //     default:    
    //         vm_error("Invalid variable type: " + std::to_string(variable_type));
    //         break;
    // }
    std::string variable_type_string = Variable::type_to_string(variable_type);

    // std::cout << "Variable name: " << name << std::endl;
    // std::cout << "Value type code: " << (int)value.type << std::endl;
    // std::cout << "Value data type: " << value.data.index() << std::endl;
    // check if the variable and value are the same type
    if(variable_type_string != get_value_type_string(value) && variable_type_string != "any"){
        vm_error("set_variable(): Cannot set variable " + name + " of type " + variable_type_string + " with value of type " + get_value_type_string(value));
    }


    // global 
    if(assignment_type == 2){ // global
        // check if we are in the main function
        if(vm->function_frames.size() != 1){
            vm_error("Cannot set global variable outside of main function");
        }
        // check if the variable already exists
        if(vm->global_variables.find(name) != vm->global_variables.end()){
            vm_error("Global variable " + name + " already exists and cannot be overwritten");
        }
        // vm->global_variables[name] = std::make_tuple(value, "global", variable_type_string);
        vm->global_variables[name] = Variable(name, Declaration_Type::DECLARATION_GLOBAL, variable_type, value); // Using the Variable class to create a global variable
    }

    // const 
    else if(assignment_type == 1){ // const
        // check if the variable already exists in the most inner scope
        if (frame->variables[frame->current_scope].find(name) != frame->variables[frame->current_scope].end()){
            vm_error("Variable " + name + " already exists in the current scope and cannot be overwritten");
        }
        // frame->variables[frame->current_scope][name] = std::make_tuple(value, "const", variable_type_string);
        frame->variables[frame->current_scope][name] = Variable(name, Declaration_Type::DECLARATION_CONST, variable_type, value); // Using the Variable class to create a const variable
    }

    // let
    else if(assignment_type == 0){ // let
        // add the variable to the current scope allowing overwriting
        // frame->variables[frame->current_scope][name] = std::make_tuple(value, "let", variable_type_string);
        frame->variables[frame->current_scope][name] = Variable(name, Declaration_Type::DECLARATION_LET, variable_type, value); // Using the Variable class to create a let variable
    }

    else{
        vm_error("Invalid assignment type: " + std::to_string(assignment_type));
    }
}

/*
Updates a variable in the current function frame, does not look in the parent frames
Looks for the variable in the closest scope, so climbs out of ifs, loops, etc.
*/
void update_variable(VM* vm, const std::string &name, Value value)
{
    function_frame *frame = get_current_function_frame(vm);
    for (int i = frame->current_scope; i >= 0; i--)
    {
        // std::cout << "Checking scope " << i << std::endl;
        // std::cout << "Variable name: " << name << std::endl;
        // //print out the variables in the scope
        // for(auto it = frame->variables[i].begin(); it != frame->variables[i].end(); it++){
        //     std::cout << "Variable: " << it->first << std::endl;
        // }
        // std::cout << "------------------------" << std::endl;


        if (frame->variables[i].find(name) != frame->variables[i].end())
        {
            // frame->variables[i][name] = value;
            // return;

            // check if the variable is const
            // if (std::get<1>(frame->variables[i][name]) == "const")
            // {
            //     vm_error("Cannot update const variable " + name);
            // }
            if(frame->variables[i][name].is_const()){
                vm_error("Cannot update const variable " + name);
            }

            // frame->variables[i][name] = std::make_tuple(value, "let");
            // check if it is hte same type
            // std::string variable_type = std::get<2>(frame->variables[i][name]);
            std::string variable_type = frame->variables[i][name].get_type_as_string(); // get the type of the variable from the Variable class
            if(variable_type != get_value_type_string(value) && variable_type != "any"){
                vm_error("Cannot update variable " + name + " of type " + variable_type + " with value of type " + get_value_type_string(value));
            }
            // frame->variables[i][name] = std::make_tuple(value, "let", variable_type);
            frame->variables[i][name].set_value(value); // Update the value using the Variable class method
            return;
        }
    }

    // check if the variable is global
    if (vm->global_variables.find(name) != vm->global_variables.end())
    {
        vm_error("Cannot update global variable " + name);
    }

    vm_error("update variable: Variable " + name + " not found");
}

/*
Gets a variable in the current function frame, does not look in the parent frames
Looks for the variable in the closest scope, so climbs out of ifs, loops, etc.
*/
Value get_variable(VM* vm, const std::string &name)
{
    function_frame *frame = get_current_function_frame(vm);
    for (int i = frame->current_scope; i >= 0; i--)
    {
        if (frame->variables[i].find(name) != frame->variables[i].end())
        {
            // return frame->variables[i][name];
            // return std::get<0>(frame->variables[i][name]); // return the value only
            // return the value from the Variable class
            // std::cout << "Getting variable: " << name << " with value type: " << frame->variables[i][name].get_value().type << std::endl; // Debugging line
            return frame->variables[i][name].get_value(); // Return the value using the Variable class method
        }
    }

    // check if the variable is global
    if (vm->global_variables.find(name) != vm->global_variables.end())
    {
        // return vm->global_variables[name];
        // return std::get<0>(vm->global_variables[name]); // return the value only
        return vm->global_variables[name].get_value(); // Return the value using the Variable class method
    }

    vm_error("get_variable: Variable " + name + " not found");
    return Value(); // To avoid warning, but this line will never be reached because of vm_error
}

/// DON'T THINK THIS IS BEING USED ANYMORE (still added stuff for const and global just in case)
/*
Will look for the variable in the current function frame and all parent frames
Looks for the variable in the closest scope, so climbs out of ifs, loops, etc. and also the closest function frame
*/
Value get_function_variable(VM* vm, const std::string &name)
{
    for(int j = (int)vm->function_frames.size() - 1; j >= 0; j--){
        function_frame *frame = vm->function_frames[j];
        for (int i = frame->current_scope; i >= 0; i--)
        {
            if (frame->variables[i].find(name) != frame->variables[i].end())
            {
                // return frame->variables[i][name];
                // return std::get<0>(frame->variables[i][name]); // return the value only
                return frame->variables[i][name].get_value(); // Return the value using the Variable class method
            }
        }
    }

    // check if the variable is global
    if (vm->global_variables.find(name) != vm->global_variables.end())
    {
        // return vm->global_variables[name];
        // return std::get<0>(vm->global_variables[name]); // return the value only
        return vm->global_variables[name].get_value(); // Return the value using the Variable class method
    }

    vm_error("get_function_variable: Variable " + name + " not found");
    return Value(); // To avoid warning, but this line will never be reached because of vm_error
}
// -------------------------------------------------------------------

// Custom types
// -------------------------------------------------------------------

/**/
void add_custom_type(VM* vm, std::string name, Value default_val, Value custom)
{
    // check if the type already exists
    for(auto it = vm->custom_types.begin(); it != vm->custom_types.end(); it++){
        if(std::get<0>(*it) == name){
            vm_error("Custom type " + name + " already exists");
        }
    }

    // add the custom type
    vm->custom_types.push_back(std::make_tuple(name, default_val, custom));
}

/**/
void add_custom_op(VM* vm, std::tuple<std::string, std::string> types, std::string name, Value op)
{
    // check if the op between the two types already exists
    if(vm->custom_ops.find(name) != vm->custom_ops.end()){
        if(vm->custom_ops[name].find(types) != vm->custom_ops[name].end()){
            vm_error("Custom op " + name + " already exists");
        }
    }

    // add the custom op
    vm->custom_ops[name][types] = op;
}

bool type_exists(VM* vm, std::string name)
{
    for(auto it = vm->custom_types.begin(); it != vm->custom_types.end(); it++){
        if(std::get<0>(*it) == name){
            return true;
        }
    }
    return false;
}

bool op_exists(VM* vm, std::string name, std::tuple<std::string, std::string> types)
{
    if(vm->custom_ops.find(name) != vm->custom_ops.end()){
        if(vm->custom_ops[name].find(types) != vm->custom_ops[name].end()){
            return true;
        }
    }
    return false;
}

Value get_custom_type(VM* vm, std::string name)
{
    for(auto it = vm->custom_types.begin(); it != vm->custom_types.end(); it++){
        if(std::get<0>(*it) == name){
            return std::get<1>(*it);
        }
    }
    vm_error("Custom type " + name + " not found");
    return Value(); // To avoid warning, but this line will never be reached because of vm_error
}

Value get_custom_op(VM* vm, std::string name, std::tuple<std::string, std::string> types)
{
    if(vm->custom_ops.find(name) != vm->custom_ops.end()){
        if(vm->custom_ops[name].find(types) != vm->custom_ops[name].end()){
            return vm->custom_ops[name][types];
        }
    }
    vm_error("Custom op " + name + " not found");
    return Value(); // To avoid warning, but this line will never be reached because of vm_error
}


/*
Takes two values and returns true if they are both maps
*/
// bool are_maps(Value a, Value b)
// {
//     return a.type == Value_Type::MAP && b.type == Value_Type::MAP;
// }

/*
Takes two values and returns true if they are both maps and have the same type (i.e. the same '__type' field)
*/
// bool are_maps_of_same_type(Value a, Value b)
// {
//     if (!are_maps(a, b))
//     {
//         return false;
//     }
//     std::map<std::string, Value> map_a = VALUE_AS_MAP(a);
//     std::map<std::string, Value> map_b = VALUE_AS_MAP(b);
//     if (map_a.find("__type") == map_a.end() || map_b.find("__type") == map_b.end())
//     {
//         return false;
//     }
//     return VALUE_AS_STRING(map_a["__type"]) == VALUE_AS_STRING(map_b["__type"]);
// }

/*
Takes two maps and an operator and calls the function associated with the operator in the custom types
*/
// void operate_on_maps(VM* vm, Value a, Value b, std::string op, bool verbose = false)
// {
//     std::map<std::string, Value> map_a = VALUE_AS_MAP(a);
//     std::map<std::string, Value> map_b = VALUE_AS_MAP(b);

//     // look for type and op in custom types
//     if(vm->custom_types.find(VALUE_AS_STRING(map_a["__type"])) == vm->custom_types.end()){
//         vm_error("Custom type " + VALUE_AS_STRING(map_a["__type"]) + " not found");
//     }

//     if(vm->custom_types[VALUE_AS_STRING(map_a["__type"])].find(op) == vm->custom_types[VALUE_AS_STRING(map_a["__type"])].end()){
//         vm_error("Custom type " + VALUE_AS_STRING(map_a["__type"]) + " does not have operator " + op);
//     }

//     push(vm, {Value_Type::MAP, map_a});
//     push(vm, {Value_Type::MAP, map_b});
//     push(vm, vm->custom_types[VALUE_AS_STRING(map_a["__type"])][op]);
//     function_call(verbose);
// }

/*
Takes a map and an operator and calls the function associated with the operator in the custom types
*/
// void operate_on_map(VM* vm, Value a, std::string op, bool verbose = false)
// {
//     std::map<std::string, Value> map_a = VALUE_AS_MAP(a);

//     // look for type and op in custom types
//     if(vm->custom_types.find(VALUE_AS_STRING(map_a["__type"])) == vm->custom_types.end()){
//         vm_error("Custom type " + VALUE_AS_STRING(map_a["__type"]) + " not found");
//     }
//     if(vm->custom_types[VALUE_AS_STRING(map_a["__type"])].find(op) == vm->custom_types[VALUE_AS_STRING(map_a["__type"])].end()){
//         vm_error("Custom type " + VALUE_AS_STRING(map_a["__type"]) + " does not have operator " + op);
//     }
//     push(vm, {Value_Type::MAP, map_a});
//     push(vm, vm->custom_types[VALUE_AS_STRING(map_a["__type"])][op]);
//     function_call(verbose);

// }
// -------------------------------------------------------------------

#endif // VM_HPP