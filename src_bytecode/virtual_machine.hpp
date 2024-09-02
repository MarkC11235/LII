#ifndef VIRUTAL_MACHINE_HPP
#define VIRUTAL_MACHINE_HPP

#include <cstdint> // int8_t

#include "Value.hpp"
#include "./std_lib/std_lib.hpp"
#include "Function.hpp"
#include "cl_exe_file.hpp"

// Data Structures ---------------------------------------------------
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
};

VM vm; // Statically allocated because only one VM is needed

// -------------------------------------------------------------------

function_frame *get_current_function_frame()
{
    return vm.function_frames.back();
}

function_frame *get_function_frame(int index)
{
    return vm.function_frames[index];
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

// Initializes the virtual machine ----------------------------------
void init_vm(cl_exe* exe, int stack_capacity = 256)
{
    vm.stack = new Value[stack_capacity];
    vm.stack_count = 0;
    vm.stack_capacity = stack_capacity;

    vm.function_frames.clear();
    vm.function_frames.push_back(create_function_frame(exe->main));

    vm.constants = exe->constants;
    vm.variable_names = exe->variable_names;
}

// -------------------------------------------------------------------

// Helper functions ---------------------------------------------------
void vm_error(const std::string &message)
{
    std::cout << "ERROR: " << message << std::endl;
    exit(1);
}

bool increase_ip(int offset)
{
    function_frame *frame = get_current_function_frame();

    if (frame->ip + offset >= frame->func->code + frame->end_of_function)
    { // Probably slow
        // if in the main function just return
        if (vm.function_frames.size() == 1)
        {
            return false;
        }
        vm_error("End of function reached without return");
    }

    frame->ip += offset;

    return true;
}

bool goto_ip(int index)
{
    function_frame *frame = get_current_function_frame();

    if (index >= frame->func->count || index < 0)
    {
        return false;
    }

    frame->ip = &frame->func->code[index];

    return true;
}

Value get_vm_constant(int index)
{
    return vm.constants[index];
}

// get ip from the current function frame
CODE_SIZE *get_ip()
{
    return get_current_function_frame()->ip;
}

// Stack operations -------------------------------------------------
void push(Value value)
{
    vm.stack[vm.stack_count++] = value;
}

Value pop()
{
    return vm.stack[--vm.stack_count];
}

Value top()
{
    return vm.stack[vm.stack_count - 1];
}

void print_stack()
{
    for (int i = 0; i < vm.stack_count; i++)
    {
        print_value(vm.stack[i]);
        std::cout << " ";
    }
    std::cout << std::endl;
}

// -------------------------------------------------------------------

// Map operations -----------------------------------------------------

// Creates a new variable in the current scope
void set_variable(const std::string &name, Value value)
{
    function_frame *frame = get_current_function_frame();
    frame->variables[frame->current_scope][name] = value;
}

// Updates a variable in the current function frame, does not look in the parent frames
// Looks for the variable in the closest scope, so climbs out of ifs, loops, etc.
void update_variable(const std::string &name, Value value)
{
    function_frame *frame = get_current_function_frame();
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
Value get_variable(const std::string &name)
{
    function_frame *frame = get_current_function_frame();
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

Value get_function_variable(const std::string &name)
{
    for(int j = 0; j < (int)vm.function_frames.size(); j++){
        function_frame *frame = vm.function_frames[j];
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

//--------------------------------------------------------------------

// Runs the virtual machine ------------------------------------------
void vm_loop(bool verbose)
{
    // Meat of the VM
    switch (*get_ip())
    {
    // Arithmetic operations
    case OpCode::OP_ADD:
    {
        Value a = pop();
        Value b = pop();
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push({Value_Type::NUMBER, std::get<double>(a.data) + std::get<double>(b.data)});
        }
        else if (a.type == Value_Type::STRING || b.type == Value_Type::STRING)
        {
            push({Value_Type::STRING, VALUE_AS_STRING(a) + VALUE_AS_STRING(b)});
        }
        else
        {
            vm_error("Invalid types for addition");
        }
        break;
    }
    case OpCode::OP_SUB:
    {
        Value a = pop();
        Value b = pop();
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push({Value_Type::NUMBER, std::get<double>(a.data) - std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for subtraction");
        }
        break;
    }
    case OpCode::OP_U_SUB:
    {
        Value a = pop();
        if (a.type == Value_Type::NUMBER)
        {
            push({Value_Type::NUMBER, -std::get<double>(a.data)});
        }
        else
        {
            vm_error("Invalid types for unary subtraction");
        }
        break;
    }
    case OpCode::OP_MUL:
    {
        Value a = pop();
        Value b = pop();
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push({Value_Type::NUMBER, std::get<double>(a.data) * std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for multiplication");
        }
        break;
    }
    case OpCode::OP_DIV:
    {
        Value a = pop();
        Value b = pop();
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            if (std::get<double>(b.data) == 0)
            {
                vm_error("Division by zero");
            }
            push({Value_Type::NUMBER, std::get<double>(a.data) / std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for division");
        }
        break;
    }
    case OpCode::OP_MOD:
    {
        Value a = pop();
        Value b = pop();
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            if (std::get<double>(b.data) == 0)
            {
                vm_error("Modulus by zero");
            }
            push({Value_Type::NUMBER, std::fmod(std::get<double>(a.data), std::get<double>(b.data))});
        }
        else
        {
            vm_error("Invalid types for modulus");
        }
        break;
    }

    // Logical operations
    // uses type coercion, check VALUE_AS_BOOL for more info
    case OpCode::OP_AND:
    {
        Value a = pop();
        Value b = pop();
        push({Value_Type::BOOL, VALUE_AS_BOOL(a) && VALUE_AS_BOOL(b)});
        break;
    }
    case OpCode::OP_OR:
    {
        Value a = pop();
        Value b = pop();
        push({Value_Type::BOOL, VALUE_AS_BOOL(a) || VALUE_AS_BOOL(b)});
        break;
    }
    case OpCode::OP_NOT:
    {
        Value a = pop();
        push({Value_Type::BOOL, !VALUE_AS_BOOL(a)});
        break;
    }

    // Comparison operations
    // uses type coercion, check VALUE_AS_BOOL, VALUE_AS_NUMBER, VALUE_AS_STRING for more info
    case OpCode::OP_EQ:
    {
        // If both are strings, compare the strings
        // Else compare the bool values
        Value a = pop();
        Value b = pop();
        if(a.type == Value_Type::STRING && b.type == Value_Type::STRING){
            push({Value_Type::BOOL, VALUE_AS_STRING(a) == VALUE_AS_STRING(b)});
        }
        else if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){
            push({Value_Type::BOOL, VALUE_AS_NUMBER(a) == VALUE_AS_NUMBER(b)});
        }
        else{
            push({Value_Type::BOOL, VALUE_AS_BOOL(a) == VALUE_AS_BOOL(b)});
        }
        break;
    }
    case OpCode::OP_NEQ:
    {
        // If both are strings, compare the strings
        // Else compare the bool values
        Value a = pop();
        Value b = pop();
        if(a.type == Value_Type::STRING && b.type == Value_Type::STRING){
            push({Value_Type::BOOL, VALUE_AS_STRING(a) != VALUE_AS_STRING(b)});
        }
        else if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){
            push({Value_Type::BOOL, VALUE_AS_NUMBER(a) != VALUE_AS_NUMBER(b)});
        }
        else{
            push({Value_Type::BOOL, VALUE_AS_BOOL(a) != VALUE_AS_BOOL(b)});
        }
        break;
    }
    case OpCode::OP_GT:
    {
        Value a = pop();
        Value b = pop();
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push({Value_Type::BOOL, std::get<double>(a.data) > std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for greater than comparison");
        }
        break;
    }
    case OpCode::OP_GTEQ:
    {
        Value a = pop();
        Value b = pop();
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push({Value_Type::BOOL, std::get<double>(a.data) >= std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for greater than or equal comparison");
        }
        break;
    }
    case OpCode::OP_LT:
    {
        Value a = pop();
        Value b = pop();
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push({Value_Type::BOOL, std::get<double>(a.data) < std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for less than comparison");
        }
        break;
    }
    case OpCode::OP_LTEQ:
    {
        Value a = pop();
        Value b = pop();
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push({Value_Type::BOOL, std::get<double>(a.data) <= std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for less than or equal comparison");
        }
        break;
    }


    // Memory operations
    case OpCode::OP_LOAD:
        push(get_vm_constant(get_ip()[1]));
        increase_ip(1);
        break;
    case OpCode::OP_STORE_VAR:
        set_variable(vm.variable_names[get_ip()[1]], pop());
        increase_ip(1);
        break;
    case OpCode::OP_UPDATE_VAR:
        update_variable(vm.variable_names[get_ip()[1]], pop());
        increase_ip(1);
        break;
    case OpCode::OP_LOAD_VAR:
        push(get_variable(vm.variable_names[get_ip()[1]]));
        increase_ip(1);
        break;
    case OpCode::OP_LOAD_FUNCTION_VAR:
        push(get_function_variable(vm.variable_names[get_ip()[1]]));
        increase_ip(1);
        break;

    // Array operations
    case OpCode::OP_CREATE_VECTOR:
    {
        push({Value_Type::VECTOR, std::vector<Value>()});
        break;
    }
    case OpCode::OP_VECTOR_PUSH:
    {
        Value value = pop();
        Value vector = get_variable(vm.variable_names[get_ip()[1]]);
        if (vector.type != Value_Type::VECTOR)
        {
            vm_error("Invalid type for vector push");
        }
        std::vector<Value> vec = VALUE_AS_VECTOR(vector);
        vec.push_back(value);
        update_variable(vm.variable_names[get_ip()[1]], {Value_Type::VECTOR, vec});
        increase_ip(1);
        break;
    }
    case OpCode::OP_UPDATE_VECTOR_ELEMENT:
    {
        Value index = pop();
        Value value = pop();
        Value vector = get_variable(vm.variable_names[get_ip()[1]]);
        if (vector.type != Value_Type::VECTOR || index.type != Value_Type::NUMBER)
        {
            vm_error("Invalid types for vector element update");
        }
        std::vector<Value> vec = VALUE_AS_VECTOR(vector);
        if (VALUE_AS_NUMBER(index) < 0 || VALUE_AS_NUMBER(index) >= vec.size())
        {
            vm_error("Index out of bounds");
        }
        vec[(int)VALUE_AS_NUMBER(index)] = value;
        update_variable(vm.variable_names[get_ip()[1]], {Value_Type::VECTOR, vec});
        increase_ip(1);
        break;
    }
    case OpCode::OP_LOAD_VECTOR_ELEMENT:
    {
        Value index = pop();
        Value vector = get_variable(vm.variable_names[get_ip()[1]]);
        if (vector.type != Value_Type::VECTOR || index.type != Value_Type::NUMBER)
        {
            vm_error("Invalid types for vector element access");
        }
        if (VALUE_AS_NUMBER(index) < 0 || VALUE_AS_NUMBER(index) >= VALUE_AS_VECTOR(vector).size())
        {
            vm_error("Index out of bounds");
        }
        std::vector<Value> vec = VALUE_AS_VECTOR(vector);
        push(vec[(int)VALUE_AS_NUMBER(index)]);
        increase_ip(1);
        break;
    }

    // Struct operations
    case OpCode::OP_CREATE_STRUCT:
    {
        push({Value_Type::STRUCT, std::map<std::string, Value>()});
        break;
    }
    case OpCode::OP_UPDATE_STRUCT_ELEMENT:
    {
        Value value = pop();
        Value struct_ = get_variable(vm.variable_names[get_ip()[1]]);
        if (struct_.type != Value_Type::STRUCT)
        {
            vm_error("Not a struct");
        }
        std::map<std::string, Value> struct_map = VALUE_AS_STRUCT(struct_);
        struct_map[vm.variable_names[get_ip()[2]]] = value;
        update_variable(vm.variable_names[get_ip()[1]], {Value_Type::STRUCT, struct_map});
        increase_ip(2);
        break;
    }
    case OpCode::OP_LOAD_STRUCT_ELEMENT:
    {
        Value struct_ = get_variable(vm.variable_names[get_ip()[1]]);
        if (struct_.type != Value_Type::STRUCT)
        {
            vm_error("Not a struct");
        }
        std::map<std::string, Value> struct_map = VALUE_AS_STRUCT(struct_);
        push(struct_map[vm.variable_names[get_ip()[2]]]);
        increase_ip(2);
        break;
    }

    // Control flow operations
    case OpCode::OP_RETURN:
    {
        if (vm.function_frames.size() == 1)
        {
            // print the return value
            if (verbose)
            {
                std::cout << "Exit Code: ";
                print_value(pop());
                std::cout << std::endl;
            }
            return;
        }

        if (verbose)
        {
            std::cout << "Returning from function" << std::endl;
        }

        // remove the current function frame
        delete get_current_function_frame();
        vm.function_frames.pop_back();

        // set the ip to the next instruction
        //get_current_function_frame()->ip += 1;

        break;
    }
    case OpCode::OP_JUMP:
        {
            if(!goto_ip(get_ip()[1]))
            {
                vm_error("Invalid jump index " + std::to_string(get_ip()[1]));
            }
        }
        break;
    case OpCode::OP_JUMP_IF_FALSE:
    {
        bool val = VALUE_AS_BOOL(pop());
        if (!val)
        {
            if(!goto_ip(get_ip()[1]))
            {
                vm_error("Invalid jump index " + std::to_string(get_ip()[1]));
            }
        }
        else
        {
            increase_ip(1);
        }
        break;
    }
    case OpCode::OP_FUNCTION_CALL:
    {
        if (verbose)
        {
            std::cout << "Calling function: " << std::endl;
        }

        // function *func = function_definitions[get_variable_name(get_ip()[1])];
        function* func = VALUE_AS_FUNCTION(pop());
        vm.function_frames.push_back(create_function_frame(func));

        get_current_function_frame()->ip = func->code - 1; // -1 because the ip will be increased by 1

        break;
    }

    case OpCode::OP_STD_LIB_CALL:
    {
        increase_ip(1);

        STD_LIB_FUNCTION_INFO func = STD_LIB_FUNCTIONS_DEFINITIONS[*get_ip()];

        // Get the arguments
        std::vector<std::any> args;
        for (int i = 0; i < (int)func.arg_types.size(); i++)
        {
            // check if the argument is the correct type
            int index = (int)func.arg_types.size() - i - 1;
            if (!LII_type_matches_cpp_type(top(), func.arg_types[index]))
            {
                vm_error("Invalid argument type. Expected: " + func.arg_types[index] + ", Got: " + get_value_type_string(top()));
            }
            // cast the argument to the c++ type
            args.push_back(cast_LII_type_to_cpp_type(pop(), func.arg_types[index]));
        }

        // reverse the arguments
        std::reverse(args.begin(), args.end());

        // Call the function
        auto result = func.function(args, func.arg_types);

        // Return value
        if (func.return_type != "void")
        {
            // check if the return value is the correct type
            if (!any_type_check(result, func.return_type))
            {
                vm_error("Invalid return type");
            }

            // cast the any type to the LII type
            if (func.return_type == "int")
            {
                push({Value_Type::NUMBER, (double)std::any_cast<int>(result)});
            }
            else if (func.return_type == "double")
            {
                push({Value_Type::NUMBER, std::any_cast<double>(result)});
            }
            else if (func.return_type == "bool")
            {
                push({Value_Type::BOOL, std::any_cast<bool>(result)});
            }
            else if (func.return_type == "std::string")
            {
                push({Value_Type::STRING, std::any_cast<std::string>(result)});
            }
            else if (func.return_type == "std::vector<Value>")
            {
                push({Value_Type::VECTOR, std::any_cast<std::vector<Value>>(result)});
            }
            else if (func.return_type == "Value")
            {
                push(std::any_cast<Value>(result));
            }
        }

        break;
    }

    // Scope operations
    case OpCode::OP_INC_SCOPE:
        get_current_function_frame()->current_scope++;
        get_current_function_frame()->variables.push_back(std::map<std::string, Value>());
        break;
    case OpCode::OP_DEC_SCOPE:
        get_current_function_frame()->current_scope--;
        get_current_function_frame()->variables.pop_back();
        break;

    // Output operations
    case OpCode::OP_PRINT:
        // pop();
        if(verbose){
            std::cout << "Output: ";
        }
        print_value(pop());
        std::cout << std::endl;
        break;

    default:
        std::cout << "ERROR: Unknown opcode" << std::endl;
        return;
    }
}

void run_vm(bool verbose = false)
{
    if (verbose)
    {
        std::cout << "Running VM" << std::endl;
    }

    while (true)
    {
        if (verbose)
        {
            function_frame *frame = get_current_function_frame();
            int instruction = frame->ip - frame->func->code; // no -1 because this is the instruction that will be executed

            std::cout << "IP: " << instruction << std::endl;
        }

        vm_loop(verbose);
        bool ok = increase_ip(1);
        if (!ok)
        {
            break;
        }
    }
}

void display_debug_info()
{
    function_frame* ff = get_current_function_frame();
    int instruction = ff->ip - ff->func->code - 1; // -1 because this is the instruction that was just executed

    std::cout << "IP: " << instruction << std::endl;
    std::cout << "Debuging Info:" << std::endl;
    

    std::cout << "\tCurrent Function: " << ff->func->name << std::endl;
    std::cout << "\tFunction Variables (outermost to innermost scope): " << std::endl;
    std::vector<std::map<std::string, Value>> variables = ff->variables;
    for(int i = 0; i < (int)variables.size(); i++)
    {
        std::cout << "\t\tScope: " << i << std::endl;
        std::map<std::string, Value> scope_variables = variables[i];
        for(const auto& pair : scope_variables)
        {
            std::cout << "\t\t\t" << pair.first << ": " << VALUE_AS_STRING(pair.second) << std::endl;
        }
    }


    std::cout << "Stack: \n";
    for(int i = 0; i < vm.stack_count; i++)
    {
        std::cout << VALUE_AS_STRING(vm.stack[i]) << std::endl;
    }

    std::cout << "--------------------------------------------------------------------" << std::endl;
}

void wait_for_continue()
{
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

void debug_vm(bool verbose = false)
{
    if (verbose)
    {
        std::cout << "Running VM" << std::endl;
    }

    while (true)
    {
        vm_loop(verbose);

        display_debug_info();
        wait_for_continue();

        bool ok = increase_ip(1);
        if (!ok)
        {
            break;
        }
    }
}


// -------------------------------------------------------------------

// Starts the interpretation process ---------------------------------
void interpret_bytecode(std::string path, bool verbose = false, bool debug = false)
{
    cl_exe* exe = read_cl_exe(path);
    init_vm(exe);
    if(debug){debug_vm(verbose);}
    else {run_vm(verbose);}

    delete exe;
}

// -------------------------------------------------------------------

#endif // VIRUTAL_MACHINE_HPP