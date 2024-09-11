#ifndef VIRUTAL_MACHINE_HPP
#define VIRUTAL_MACHINE_HPP

#include <cstdint> // int8_t

#include "Value.hpp"
#include "./std_lib/std_lib.hpp"
#include "Function.hpp"
#include "cl_exe_file.hpp"
#include "VM.hpp"
#include "jit.hpp"

// Initializes the virtual machine ----------------------------------
void init_vm(cl_exe* exe, bool jit, int stack_capacity = 256)
{
    vm.stack = new Value[stack_capacity];
    vm.stack_count = 0;
    vm.stack_capacity = stack_capacity;

    vm.function_frames.clear();
    vm.function_frames.push_back(create_function_frame(exe->main));

    vm.constants = exe->constants;
    vm.variable_names = exe->variable_names;

    vm.jit = jit;
}

// -------------------------------------------------------------------

// Runs the virtual machine ------------------------------------------
void vm_loop(bool verbose)
{
    // Meat of the VM
    switch (*get_ip(&vm))
    {
    // Arithmetic operations
    case OpCode::OP_ADD:
    {
        Value a = pop(&vm);
        Value b = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push(&vm, {Value_Type::NUMBER, std::get<double>(a.data) + std::get<double>(b.data)});
        }
        else if (a.type == Value_Type::STRING || b.type == Value_Type::STRING)
        {
            push(&vm, {Value_Type::STRING, VALUE_AS_STRING(a) + VALUE_AS_STRING(b)});
        }
        else
        {
            vm_error("Invalid types for addition");
        }
        break;
    }
    case OpCode::OP_SUB:
    {
        Value a = pop(&vm);
        Value b = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push(&vm, {Value_Type::NUMBER, std::get<double>(a.data) - std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for subtraction");
        }
        break;
    }
    case OpCode::OP_U_SUB:
    {
        Value a = pop(&vm);
        if (a.type == Value_Type::NUMBER)
        {
            push(&vm, {Value_Type::NUMBER, -std::get<double>(a.data)});
        }
        else
        {
            vm_error("Invalid types for unary subtraction");
        }
        break;
    }
    case OpCode::OP_MUL:
    {
        Value a = pop(&vm);
        Value b = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push(&vm, {Value_Type::NUMBER, std::get<double>(a.data) * std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for multiplication");
        }
        break;
    }
    case OpCode::OP_DIV:
    {
        Value a = pop(&vm);
        Value b = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            if (std::get<double>(b.data) == 0)
            {
                vm_error("Division by zero");
            }
            push(&vm, {Value_Type::NUMBER, std::get<double>(a.data) / std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for division");
        }
        break;
    }
    case OpCode::OP_MOD:
    {
        Value a = pop(&vm);
        Value b = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            if (std::get<double>(b.data) == 0)
            {
                vm_error("Modulus by zero");
            }
            push(&vm, {Value_Type::NUMBER, std::fmod(std::get<double>(a.data), std::get<double>(b.data))});
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
        Value a = pop(&vm);
        Value b = pop(&vm);
        push(&vm, {Value_Type::BOOL, VALUE_AS_BOOL(a) && VALUE_AS_BOOL(b)});
        break;
    }
    case OpCode::OP_OR:
    {
        Value a = pop(&vm);
        Value b = pop(&vm);
        push(&vm, {Value_Type::BOOL, VALUE_AS_BOOL(a) || VALUE_AS_BOOL(b)});
        break;
    }
    case OpCode::OP_NOT:
    {
        Value a = pop(&vm);
        push(&vm, {Value_Type::BOOL, !VALUE_AS_BOOL(a)});
        break;
    }

    // Comparison operations
    // uses type coercion, check VALUE_AS_BOOL, VALUE_AS_NUMBER, VALUE_AS_STRING for more info
    case OpCode::OP_EQ:
    {
        // If both are strings, compare the strings
        // Else compare the bool values
        Value a = pop(&vm);
        Value b = pop(&vm);
        if(a.type == Value_Type::STRING && b.type == Value_Type::STRING){
            push(&vm, {Value_Type::BOOL, VALUE_AS_STRING(a) == VALUE_AS_STRING(b)});
        }
        else if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){
            push(&vm, {Value_Type::BOOL, VALUE_AS_NUMBER(a) == VALUE_AS_NUMBER(b)});
        }
        else{
            push(&vm, {Value_Type::BOOL, VALUE_AS_BOOL(a) == VALUE_AS_BOOL(b)});
        }
        break;
    }
    case OpCode::OP_NEQ:
    {
        // If both are strings, compare the strings
        // Else compare the bool values
        Value a = pop(&vm);
        Value b = pop(&vm);
        if(a.type == Value_Type::STRING && b.type == Value_Type::STRING){
            push(&vm, {Value_Type::BOOL, VALUE_AS_STRING(a) != VALUE_AS_STRING(b)});
        }
        else if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){
            push(&vm, {Value_Type::BOOL, VALUE_AS_NUMBER(a) != VALUE_AS_NUMBER(b)});
        }
        else{
            push(&vm, {Value_Type::BOOL, VALUE_AS_BOOL(a) != VALUE_AS_BOOL(b)});
        }
        break;
    }
    case OpCode::OP_GT:
    {
        Value a = pop(&vm);
        Value b = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push(&vm, {Value_Type::BOOL, std::get<double>(a.data) > std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for greater than comparison");
        }
        break;
    }
    case OpCode::OP_GTEQ:
    {
        Value a = pop(&vm);
        Value b = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push(&vm, {Value_Type::BOOL, std::get<double>(a.data) >= std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for greater than or equal comparison");
        }
        break;
    }
    case OpCode::OP_LT:
    {
        Value a = pop(&vm);
        Value b = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push(&vm, {Value_Type::BOOL, std::get<double>(a.data) < std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for less than comparison");
        }
        break;
    }
    case OpCode::OP_LTEQ:
    {
        Value a = pop(&vm);
        Value b = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push(&vm, {Value_Type::BOOL, std::get<double>(a.data) <= std::get<double>(b.data)});
        }
        else
        {
            vm_error("Invalid types for less than or equal comparison");
        }
        break;
    }


    // Memory operations
    case OpCode::OP_LOAD:
        push(&vm, get_vm_constant(&vm, get_ip(&vm)[1]));
        increase_ip(&vm, 1);
        break;
    case OpCode::OP_STORE_VAR:
        set_variable(&vm, vm.variable_names[get_ip(&vm)[1]], pop(&vm));
        increase_ip(&vm, 1);
        break;
    case OpCode::OP_UPDATE_VAR:
        update_variable(&vm, vm.variable_names[get_ip(&vm)[1]], pop(&vm));
        increase_ip(&vm, 1);
        break;
    case OpCode::OP_LOAD_VAR:
        push(&vm, get_variable(&vm, vm.variable_names[get_ip(&vm)[1]]));
        increase_ip(&vm, 1);
        break;
    case OpCode::OP_LOAD_FUNCTION_VAR:
        push(&vm, get_function_variable(&vm, vm.variable_names[get_ip(&vm)[1]]));
        increase_ip(&vm, 1);
        break;

    // Array operations
    case OpCode::OP_CREATE_VECTOR:
    {
        push(&vm, {Value_Type::VECTOR, std::vector<Value>()});
        break;
    }
    case OpCode::OP_VECTOR_PUSH:
    {
        Value value = pop(&vm);
        Value vector = get_variable(&vm, vm.variable_names[get_ip(&vm)[1]]);
        if (vector.type != Value_Type::VECTOR)
        {
            vm_error("Invalid type for vector push");
        }
        std::vector<Value> vec = VALUE_AS_VECTOR(vector);
        vec.push_back(value);
        update_variable(&vm, vm.variable_names[get_ip(&vm)[1]], {Value_Type::VECTOR, vec});
        increase_ip(&vm, 1);
        break;
    }
    case OpCode::OP_UPDATE_VECTOR_ELEMENT:
    {
        Value index = pop(&vm);
        Value value = pop(&vm);
        Value vector = get_variable(&vm, vm.variable_names[get_ip(&vm)[1]]);
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
        update_variable(&vm, vm.variable_names[get_ip(&vm)[1]], {Value_Type::VECTOR, vec});
        increase_ip(&vm, 1);
        break;
    }
    case OpCode::OP_LOAD_VECTOR_ELEMENT:
    {
        Value index = pop(&vm);
        Value vector = get_variable(&vm, vm.variable_names[get_ip(&vm)[1]]);
        if (vector.type != Value_Type::VECTOR || index.type != Value_Type::NUMBER)
        {
            vm_error("Invalid types for vector element access");
        }
        if (VALUE_AS_NUMBER(index) < 0 || VALUE_AS_NUMBER(index) >= VALUE_AS_VECTOR(vector).size())
        {
            vm_error("Index out of bounds");
        }
        std::vector<Value> vec = VALUE_AS_VECTOR(vector);
        push(&vm, vec[(int)VALUE_AS_NUMBER(index)]);
        increase_ip(&vm, 1);
        break;
    }

    // Struct operations
    case OpCode::OP_CREATE_STRUCT:
    {
        push(&vm, {Value_Type::STRUCT, std::map<std::string, Value>()});
        break;
    }
    case OpCode::OP_UPDATE_STRUCT_ELEMENT:
    {
        Value value = pop(&vm);
        Value struct_ = get_variable(&vm, vm.variable_names[get_ip(&vm)[1]]);
        if (struct_.type != Value_Type::STRUCT)
        {
            vm_error("Not a struct");
        }
        std::map<std::string, Value> struct_map = VALUE_AS_STRUCT(struct_);
        struct_map[vm.variable_names[get_ip(&vm)[2]]] = value;
        update_variable(&vm, vm.variable_names[get_ip(&vm)[1]], {Value_Type::STRUCT, struct_map});
        increase_ip(&vm, 2);
        break;
    }
    case OpCode::OP_LOAD_STRUCT_ELEMENT:
    {
        Value struct_ = get_variable(&vm, vm.variable_names[get_ip(&vm)[1]]);
        if (struct_.type != Value_Type::STRUCT)
        {
            vm_error("Not a struct");
        }
        std::map<std::string, Value> struct_map = VALUE_AS_STRUCT(struct_);
        push(&vm, struct_map[vm.variable_names[get_ip(&vm)[2]]]);
        increase_ip(&vm, 2);
        break;
    }
    case OpCode::OP_ACCESS:
    {
        Value index = pop(&vm);
        Value obj = pop(&vm);
        if (obj.type == Value_Type::STRUCT)
        {
            std::map<std::string, Value> struct_map = VALUE_AS_STRUCT(obj);
            //check if the key exists
            if (struct_map.find(VALUE_AS_STRING(index)) == struct_map.end())
            {
                vm_error("Key does not exist in struct");
            }
            push(&vm, struct_map[VALUE_AS_STRING(index)]);
        }
        else if (obj.type == Value_Type::VECTOR)
        {
            std::vector<Value> vec = VALUE_AS_VECTOR(obj);
            if (index.type != Value_Type::NUMBER)
            {
                vm_error("Invalid index type for vector access");
            }
            if (VALUE_AS_NUMBER(index) < 0 || VALUE_AS_NUMBER(index) >= vec.size())
            {
                vm_error("Index out of bounds");
            }
            push(&vm, vec[(int)VALUE_AS_NUMBER(index)]);
        }
        else
        {
            vm_error("Invalid type for access");
        }
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
                print_value(pop(&vm));
                std::cout << std::endl;
            }
            return;
        }

        if (verbose)
        {
            std::cout << "Returning from function" << std::endl;
        }

        // remove the current function frame
        delete get_current_function_frame(&vm);
        vm.function_frames.pop_back();

        // set the ip to the next instruction
        //get_current_function_frame()->ip += 1;

        break;
    }
    case OpCode::OP_JUMP:
        {
            if(!goto_ip(&vm, get_ip(&vm)[1]))
            {
                vm_error("Invalid jump index " + std::to_string(get_ip(&vm)[1]));
            }
        }
        break;
    case OpCode::OP_JUMP_IF_FALSE:
    {
        bool val = VALUE_AS_BOOL(pop(&vm));
        if (!val)
        {
            if(!goto_ip(&vm, get_ip(&vm)[1]))
            {
                vm_error("Invalid jump index " + std::to_string(get_ip(&vm)[1]));
            }
        }
        else
        {
            increase_ip(&vm, 1);
        }
        break;
    }
    case OpCode::OP_FUNCTION_CALL:
    {
        if (verbose)
        {
            std::cout << "Calling function: " << std::endl;
        }

        function* func = VALUE_AS_FUNCTION(pop(&vm));

        func->times_called++; // for jit compilation

        if(vm.jit && func->times_called == CALLS_TO_JIT){
            if(verbose){
                std::cout << "JIT compiling function: " << func->name << std::endl;
            }        
            jit_compile_function(&vm, func);
        }

        if(vm.jit && func->jit_index != -1){
            if(verbose){
                std::cout << "Calling JIT function: " << func->jit_index << std::endl;
            }
            vm.function_frames.push_back(create_function_frame(func));
            jit_run_function(&vm, func->jit_index);
        }
        else{
            vm.function_frames.push_back(create_function_frame(func));

            get_current_function_frame(&vm)->ip = func->code - 1; // -1 because the ip will be increased by 1
        }

        break;
    }

    case OpCode::OP_STD_LIB_CALL:
    {
        increase_ip(&vm, 1);

        STD_LIB_FUNCTION_INFO func = STD_LIB_FUNCTIONS_DEFINITIONS[*get_ip(&vm)];

        // Get the arguments
        std::vector<std::any> args;
        for (int i = 0; i < (int)func.arg_types.size(); i++)
        {
            // check if the argument is the correct type
            int index = (int)func.arg_types.size() - i - 1;
            if (!LII_type_matches_cpp_type(top(&vm), func.arg_types[index]))
            {
                vm_error("Invalid argument type. Expected: " + func.arg_types[index] + ", Got: " + get_value_type_string(top(&vm)));
            }
            // cast the argument to the c++ type
            args.push_back(cast_LII_type_to_cpp_type(pop(&vm), func.arg_types[index]));
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
                push(&vm, {Value_Type::NUMBER, (double)std::any_cast<int>(result)});
            }
            else if (func.return_type == "double")
            {
                push(&vm, {Value_Type::NUMBER, std::any_cast<double>(result)});
            }
            else if (func.return_type == "bool")
            {
                push(&vm, {Value_Type::BOOL, std::any_cast<bool>(result)});
            }
            else if (func.return_type == "std::string")
            {
                push(&vm, {Value_Type::STRING, std::any_cast<std::string>(result)});
            }
            else if (func.return_type == "std::vector<Value>")
            {
                push(&vm, {Value_Type::VECTOR, std::any_cast<std::vector<Value>>(result)});
            }
            else if (func.return_type == "Value")
            {
                push(&vm, std::any_cast<Value>(result));
            }
        }

        break;
    }

    // Scope operations
    case OpCode::OP_INC_SCOPE:
        get_current_function_frame(&vm)->current_scope++;
        get_current_function_frame(&vm)->variables.push_back(std::map<std::string, Value>());
        break;
    case OpCode::OP_DEC_SCOPE:
        get_current_function_frame(&vm)->current_scope--;
        get_current_function_frame(&vm)->variables.pop_back();
        break;

    // Output operations
    case OpCode::OP_PRINT:
        // pop(&vm);
        if(verbose){
            std::cout << "Output: ";
        }
        print_value(pop(&vm));
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
            function_frame *frame = get_current_function_frame(&vm);
            int instruction = frame->ip - frame->func->code; // no -1 because this is the instruction that will be executed

            std::cout << "IP: " << instruction << std::endl;
        }

        vm_loop(verbose);
        bool ok = increase_ip(&vm, 1);
        if (!ok)
        {
            break;
        }
    }
}

void display_debug_info()
{
    function_frame* ff = get_current_function_frame(&vm);
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

        bool ok = increase_ip(&vm, 1);
        if (!ok)
        {
            break;
        }
    }
}

// -------------------------------------------------------------------

// Starts the interpretation process ---------------------------------
void interpret_bytecode(std::string path, bool verbose = false, bool debug = false, bool jit = false)
{
    cl_exe* exe = read_cl_exe(path);
    init_vm(exe, jit);
    if(debug){debug_vm(verbose);}
    else {run_vm(verbose);}

    delete exe;
}

// -------------------------------------------------------------------

#endif // VIRUTAL_MACHINE_HPP