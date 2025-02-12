#ifndef VIRUTAL_MACHINE_HPP
#define VIRUTAL_MACHINE_HPP

#include <cstdint> // int8_t

#include "Value.hpp"
#include "./std_lib/std_lib.hpp"
#include "Function.hpp"
#include "cl_exe_file.hpp"
#include "VM.hpp"
// #include "jit.hpp"

// Initializes the virtual machine ----------------------------------
/*
VM constructor from a cl_exe object
*/
void init_vm(cl_exe* exe, bool jit, int calls_to_jit = 10, int stack_capacity = 256, int args_count = 0, std::vector<std::string> args = std::vector<std::string>())
{
    vm.stack = new Value[stack_capacity];
    vm.stack_count = 0;
    vm.stack_capacity = stack_capacity;

    vm.function_frames.clear();
    vm.function_frames.push_back(create_function_frame(exe->main));

    vm.constants = exe->constants;
    vm.variable_names = exe->variable_names;

    // vm.jit = jit;
    // vm.calls_to_jit = calls_to_jit;

    // Add argc and argv to the main function as variables
    set_variable(&vm, "argc", {Value_Type::NUMBER, (double)args_count});
    std::vector<Value> argv;
    for (std::string arg : args)
    {
        argv.push_back({Value_Type::STRING, arg});
    }
    set_variable(&vm, "argv", {Value_Type::VECTOR, argv});
}
// -------------------------------------------------------------------

// Runs the virtual machine ------------------------------------------
/*
Switches on the current instruction and executes the corresponding operation
*/
void vm_loop(bool verbose)
{
    // Meat of the VM
    switch (*get_ip(&vm))
    {
    // Arithmetic operations
    case OpCode::OP_ADD:
    {
        Value b = pop(&vm);
        Value a = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push(&vm, {Value_Type::NUMBER, std::get<double>(a.data) + std::get<double>(b.data)});
        }
        else if (a.type == Value_Type::STRING || b.type == Value_Type::STRING)
        {
            push(&vm, {Value_Type::STRING, VALUE_AS_STRING(a) + VALUE_AS_STRING(b)});
        }
        else if (are_maps_of_same_type(a, b))
        {
            operate_on_maps(&vm, a, b, "+", verbose);
        }
        else if (a.type == Value_Type::MAP && b.type == Value_Type::MAP)
        {
            std::map<std::string, Value> map_a = VALUE_AS_MAP(a);
            std::map<std::string, Value> map_b = VALUE_AS_MAP(b);

            for(const auto& pair : map_b)
            {
                map_a[pair.first] = pair.second;
            }
            push(&vm, {Value_Type::MAP, map_a});
        }
        else if (a.type == Value_Type::VECTOR && b.type == Value_Type::VECTOR)
        {
            std::vector<Value> vec_a = VALUE_AS_VECTOR(a);
            std::vector<Value> vec_b = VALUE_AS_VECTOR(b);
            vec_a.insert(vec_a.end(), vec_b.begin(), vec_b.end());
            push(&vm, {Value_Type::VECTOR, vec_a});
        }
        else
        {
            vm_error("Invalid types for addition");
        }
        break;
    }
    case OpCode::OP_SUB:
    {
        Value b = pop(&vm);
        Value a = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push(&vm, {Value_Type::NUMBER, std::get<double>(a.data) - std::get<double>(b.data)});
        }
        else if (are_maps_of_same_type(a, b))
        {
            operate_on_maps(&vm, a, b, "-", verbose);
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
        Value b = pop(&vm);
        Value a = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push(&vm, {Value_Type::NUMBER, std::get<double>(a.data) * std::get<double>(b.data)});
        }
        else if(a.type == Value_Type::STRING && b.type == Value_Type::NUMBER)
        {
            std::string str = VALUE_AS_STRING(a);
            int times = (int)VALUE_AS_NUMBER(b);
            std::string result = "";
            for (int i = 0; i < times; i++)
            {
                result += str;
            }
            push(&vm, {Value_Type::STRING, result});
        }
        else if(a.type == Value_Type::NUMBER && b.type == Value_Type::STRING)
        {
            std::string str = VALUE_AS_STRING(b);
            int times = (int)VALUE_AS_NUMBER(a);
            std::string result = "";
            for (int i = 0; i < times; i++)
            {
                result += str;
            }
            push(&vm, {Value_Type::STRING, result});
        }
        else if(a.type == Value_Type::VECTOR && b.type == Value_Type::NUMBER)
        {
            std::vector<Value> vec = VALUE_AS_VECTOR(a);
            int times = (int)VALUE_AS_NUMBER(b);
            std::vector<Value> result;
            result.reserve(vec.size() * times);
            for (int i = 0; i < times; i++)
            {
                result.insert(result.end(), vec.begin(), vec.end());
            }
            push(&vm, {Value_Type::VECTOR, result});
        }
        else if(a.type == Value_Type::NUMBER && b.type == Value_Type::VECTOR)
        {
            std::vector<Value> vec = VALUE_AS_VECTOR(b);
            int times = (int)VALUE_AS_NUMBER(a);
            std::vector<Value> result = {};
            for (int i = 0; i < times; i++)
            {
                for (const auto& value : vec)
                {
                    result.push_back(value);
                }
            }
            push(&vm, {Value_Type::VECTOR, result});
        }
        else if (are_maps_of_same_type(a, b))
        {
            operate_on_maps(&vm, a, b, "*", verbose);
        }
        else
        {
            vm_error("Invalid types for multiplication");
        }
        break;
    }
    case OpCode::OP_DIV:
    {
        Value b = pop(&vm);
        Value a = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            if (std::get<double>(b.data) == 0)
            {
                vm_error("Division by zero");
            }
            push(&vm, {Value_Type::NUMBER, std::get<double>(a.data) / std::get<double>(b.data)});
        }
        else if (are_maps_of_same_type(a, b))
        {
            operate_on_maps(&vm, a, b, "/", verbose);
        }
        else
        {
            vm_error("Invalid types for division");
        }
        break;
    }
    case OpCode::OP_MOD:
    {
        Value b = pop(&vm);
        Value a = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            if (std::get<double>(b.data) == 0)
            {
                vm_error("Modulus by zero");
            }
            push(&vm, {Value_Type::NUMBER, std::fmod(std::get<double>(a.data), std::get<double>(b.data))});
        }
        else if (are_maps_of_same_type(a, b))
        {
            operate_on_maps(&vm, a, b, "%", verbose);
        }
        else
        {
            vm_error("Invalid types for modulus");
        }
        break;
    }
    case OpCode::OP_EXP:
    {
        Value b = pop(&vm);
        Value a = pop(&vm);
        if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)
        {
            push(&vm, {Value_Type::NUMBER, std::pow(std::get<double>(a.data), std::get<double>(b.data))});
        }
        else if (are_maps_of_same_type(a, b))
        {
            operate_on_maps(&vm, a, b, "^", verbose);
        }
        else
        {
            vm_error("Invalid types for exponentiation");
        }
        break;
    }

    // Logical operations
    // uses type coercion, check VALUE_AS_BOOL for more info
    case OpCode::OP_AND:
    {
        Value b = pop(&vm);
        Value a = pop(&vm);
        push(&vm, {Value_Type::BOOL, VALUE_AS_BOOL(a) && VALUE_AS_BOOL(b)});
        break;
    }
    case OpCode::OP_OR:
    {
        Value b = pop(&vm);
        Value a = pop(&vm);
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
        Value b = pop(&vm);
        Value a = pop(&vm);
        if(a.type == Value_Type::STRING && b.type == Value_Type::STRING){
            push(&vm, {Value_Type::BOOL, VALUE_AS_STRING(a) == VALUE_AS_STRING(b)});
        }
        else if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){
            push(&vm, {Value_Type::BOOL, VALUE_AS_NUMBER(a) == VALUE_AS_NUMBER(b)});
        }
        // Needs to be in this order 
        // If both are null, return true
        else if(a.type == Value_Type::NULL_VALUE && b.type == Value_Type::NULL_VALUE){
            push(&vm, {Value_Type::BOOL, true});
        }
        // If one of them is null, return false
        else if(a.type == Value_Type::NULL_VALUE || b.type == Value_Type::NULL_VALUE){
            push(&vm, {Value_Type::BOOL, false});
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
        Value b = pop(&vm);
        Value a = pop(&vm);
        if(a.type == Value_Type::STRING && b.type == Value_Type::STRING){
            push(&vm, {Value_Type::BOOL, VALUE_AS_STRING(a) != VALUE_AS_STRING(b)});
        }
        else if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){
            push(&vm, {Value_Type::BOOL, VALUE_AS_NUMBER(a) != VALUE_AS_NUMBER(b)});
        }
        // Needs to be in this order 
        // If both are null, return false        
        else if(a.type == Value_Type::NULL_VALUE && b.type == Value_Type::NULL_VALUE){
            push(&vm, {Value_Type::BOOL, false});
        }
        // If one of them is null, return true
        else if(a.type == Value_Type::NULL_VALUE || b.type == Value_Type::NULL_VALUE){
            push(&vm, {Value_Type::BOOL, true});
        }
        else{
            push(&vm, {Value_Type::BOOL, VALUE_AS_BOOL(a) != VALUE_AS_BOOL(b)});
        }
        break;
    }
    case OpCode::OP_GT:
    {
        Value b = pop(&vm);
        Value a = pop(&vm);
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
        Value b = pop(&vm);
        Value a = pop(&vm);
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
        Value b = pop(&vm);
        Value a = pop(&vm);
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
        Value b = pop(&vm);
        Value a = pop(&vm);
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
        Value vector = pop(&vm);
        if (vector.type != Value_Type::VECTOR)
        {
            vm_error("Invalid type for vector push");
        }
        std::vector<Value> vec = VALUE_AS_VECTOR(vector);
        vec.push_back(value);
        push(&vm, {Value_Type::VECTOR, vec});
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

    // Map operations
    case OpCode::OP_CREATE_MAP:
    {
        push(&vm, {Value_Type::MAP, std::map<std::string, Value>()});
        break;
    }
    case OpCode::OP_UPDATE_MAP_ELEMENT:
    {
        Value value = pop(&vm);
        Value map_ = get_variable(&vm, vm.variable_names[get_ip(&vm)[1]]);
        if (map_.type != Value_Type::MAP)
        {
            vm_error("Not a map");
        }
        std::map<std::string, Value> map_map = VALUE_AS_MAP(map_);
        map_map[vm.variable_names[get_ip(&vm)[2]]] = value;
        update_variable(&vm, vm.variable_names[get_ip(&vm)[1]], {Value_Type::MAP, map_map});
        increase_ip(&vm, 2);
        break;
    }
    case OpCode::OP_LOAD_MAP_ELEMENT:
    {
        Value map_ = get_variable(&vm, vm.variable_names[get_ip(&vm)[1]]);
        if (map_.type != Value_Type::MAP)
        {
            vm_error("Not a map");
        }
        std::map<std::string, Value> map_map = VALUE_AS_MAP(map_);
        push(&vm, map_map[vm.variable_names[get_ip(&vm)[2]]]);
        increase_ip(&vm, 2);
        break;
    }
    case OpCode::OP_ACCESS:
    {
        Value index = pop(&vm);
        Value obj = pop(&vm);
        if (obj.type == Value_Type::MAP)
        {
            std::map<std::string, Value> map_map = VALUE_AS_MAP(obj);
            //check if the key exists
            if (map_map.find(VALUE_AS_STRING(index)) == map_map.end())
            {
                vm_error("Key does not exist in map");
            }
            push(&vm, map_map[VALUE_AS_STRING(index)]);
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
    case OpCode::OP_ACCESS_FOR_UPDATE:
    {
        Value index = pop(&vm);
        Value obj = pop(&vm);
        push(&vm, obj);
        push(&vm, index);

        if (obj.type == Value_Type::MAP)
        {
            std::map<std::string, Value> map_map = VALUE_AS_MAP(obj);
            //check if the key exists
            if (map_map.find(VALUE_AS_STRING(index)) == map_map.end())
            {
                vm_error("Key does not exist in map");
            }
            push(&vm, map_map[VALUE_AS_STRING(index)]);
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
    case OpCode::OP_UPDATE_STACK_ELEMENT:
    {
        Value value = pop(&vm);
        Value index = pop(&vm);
        Value obj = pop(&vm);
    
        if (obj.type == Value_Type::MAP)
        {
            std::map<std::string, Value> map_map = VALUE_AS_MAP(obj);
            //if the key does not exist, it will be added
            map_map[VALUE_AS_STRING(index)] = value;
            push(&vm, {Value_Type::MAP, map_map});
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
            vec[(int)VALUE_AS_NUMBER(index)] = value;
            push(&vm, {Value_Type::VECTOR, vec});
        }
        else
        {
            vm_error("Invalid type for access");
        }
        break;
    }
    case OpCode::OP_ACCESS_STACK_ELEMENT_QUEUE:
    {
        Value obj = pop(&vm);
        int index = VALUE_AS_NUMBER(pop(&vm));
        if (obj.type == Value_Type::MAP)
        {
            std::map<std::string, Value> map_map = VALUE_AS_MAP(obj);
            if (map_map.size() == 0)
            {
                vm_error("Map is empty");
            }
            auto it = map_map.begin();
            //get the first key:value pair
            Value key = {Value_Type::STRING, it->first};
            Value value = it->second;
            map_map.erase(it);

            push(&vm, {Value_Type::NUMBER, (double)(index + 1)});
            push(&vm, {Value_Type::MAP, map_map});
            push(&vm, value);
            push(&vm, key);
        }
        else if (obj.type == Value_Type::VECTOR)
        {
            std::vector<Value> vec = VALUE_AS_VECTOR(obj);
            if (vec.size() == 0)
            {
                vm_error("Vector is empty");
            }
            Value value = vec[0];
            vec.erase(vec.begin());
            push(&vm, {Value_Type::NUMBER, (double)(index + 1)}); // continue to use as the index
            push(&vm, {Value_Type::VECTOR, vec});
            push(&vm, value);
            push(&vm, {Value_Type::NUMBER, (double)(index)}); // use as the key
        }
        else
        {
            vm_error("Invalid type for access");
        }
        break;
    }
    case OpCode::OP_NOT_EMPTY:
    {
        Value obj = pop(&vm);
        push(&vm, obj);
        if (obj.type == Value_Type::MAP)
        {
            std::map<std::string, Value> map_map = VALUE_AS_MAP(obj);
            push(&vm, {Value_Type::BOOL, map_map.size() > 0});
        }
        else if (obj.type == Value_Type::VECTOR)
        {
            std::vector<Value> vec = VALUE_AS_VECTOR(obj);
            push(&vm, {Value_Type::BOOL, vec.size() > 0});
        }
        else
        {
            vm_error("Invalid type for not empty check");
        }
        break;
    }
    case OpCode::OP_POP:
    {
        Value obj = pop(&vm);
        break;
    }
    case OpCode::OP_DEFINE_OP_FOR_TYPE:
    {
        Value op = pop(&vm);
        Value type = pop(&vm);
        Value func = pop(&vm);
        if (op.type != Value_Type::STRING || type.type != Value_Type::STRING || func.type != Value_Type::FUNCTION)
        {
            vm_error("Invalid types for custom operation");
        }

        vm.custom_types[VALUE_AS_STRING(type)][VALUE_AS_STRING(op)] = func;
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
        function_call(verbose);

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
            else if (func.return_type == "std::map<std::string, Value>")
            {
                push(&vm, {Value_Type::MAP, std::any_cast<std::map<std::string, Value>>(result)});
            }
            else if (func.return_type == "Value")
            {
                push(&vm, std::any_cast<Value>(result));
            }
            else{
                vm_error("Invalid return type: " + func.return_type);
            }
        }
        else
        {
            vm_error("Invalid return type: void");
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

/*
Loops and calls vm_loop until the end of the program is reached
The end of the program is reached when the current function frame is 
the main function frame and the ip is at the end of the code or a return statement in the main function is reached
*/
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

/*
Called on each step of the VM when in debug mode
*/
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

/*
Wait for the user to press enter
*/
void wait_for_continue()
{
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

/*
Runs the VM loop in debug mode
*/
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

/*
Entry point for the virtual machine
*/
void interpret_bytecode(std::string path, bool verbose = false, bool debug = false, bool jit = false, int calls_to_jit = 10, int stack_capacity = 256, int args_count = 0, std::vector<std::string> args = std::vector<std::string>{})
{
    cl_exe* exe = read_cl_exe(path);
    init_vm(exe, jit, calls_to_jit, stack_capacity, args_count, args);
    if(debug){debug_vm(verbose);}
    else {run_vm(verbose);}

    delete exe;
}

#endif // VIRUTAL_MACHINE_HPP