#ifndef JIT_HPP
#define JIT_HPP

#include <vector>
#include <iostream>
#include <dlfcn.h> // For dlopen, dlsym, dlclose
#include <unistd.h>
#include <limits.h>

#include "VM.hpp"
#include "virtual_machine.hpp"
#include "Value.hpp"
#include "opcodes.hpp"

/*
Adds the function to the jit_functions vector
*/
void jit_add_function(VM* vm, JIT_FUNCTION func)
{
    vm->jit_functions.push_back(func);
}

/*
Calls a jit function at the given index
*/
void jit_run_function(VM* vm, int index)
{
    vm->jit_functions[index](vm);
}

/*
Creates the boilerplate code for the JIT function
Then writes the text representation of what the switch case in vm_loop would look like for each opcode
Then compiles the code into a shared object file
*/
void jit_compile_function(VM* vm, function* func)
{
    std::string jit_name = "jit_" + std::to_string(vm->jit_functions.size());
    std::string program = R"(
                        #include <iostream>
                        #include "../src_bytecode/VM.hpp"
                        #include "../src_bytecode/Value.hpp"
                        #include "../src_bytecode/std_lib/std_lib.hpp"
                        #include "../src_bytecode/virtual_machine.hpp"
                        #include "../src_bytecode/jit.hpp"
                        extern "C" void )" + jit_name + R"((VM* vm){)";

    for(int i = 0; i < func->count; i++){
        program += "\nlabel_" + std::to_string(i) + ": \n";
        // Add a comment with the Opcode name
        program += "// " + opcode_to_string(func->code[i]) + "\n";
        program += "{\n";
        switch (func->code[i])
        {
        // Arithmetic operations
        case OpCode::OP_ADD:
        {
            program += R"(
            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {
                push(vm, {Value_Type::NUMBER, std::get<double>(a.data) + std::get<double>(b.data)});
            }
            else if (a.type == Value_Type::STRING || b.type == Value_Type::STRING)
            {
                push(vm, {Value_Type::STRING, VALUE_AS_STRING(a) + VALUE_AS_STRING(b)});
            }
            else if (a.type == Value_Type::MAP && b.type == Value_Type::MAP)
            {
                std::map<std::string, Value> map_a = VALUE_AS_MAP(a);
                std::map<std::string, Value> map_b = VALUE_AS_MAP(b);
                for(const auto& pair : map_b)
                {
                    map_a[pair.first] = pair.second;
                }
                push(vm, {Value_Type::MAP, map_a});
            }
            else if (a.type == Value_Type::VECTOR && b.type == Value_Type::VECTOR)
            {
                std::vector<Value> vec_a = VALUE_AS_VECTOR(a);
                std::vector<Value> vec_b = VALUE_AS_VECTOR(b);
                for (const auto& value : vec_b)
                {
                    vec_a.push_back(value);
                }
                push(vm, {Value_Type::VECTOR, vec_a});
            }
            else
            {
                vm_error("Invalid types for addition");
            })";
            break;
        }
        case OpCode::OP_SUB:
        {
            program += R"(
            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {
                push(vm, {Value_Type::NUMBER, std::get<double>(a.data) - std::get<double>(b.data)});
            }
            else
            {
                vm_error("Invalid types for subtraction");
            })";
            break;
        }
        case OpCode::OP_U_SUB:
        {
            program += R"(
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER)                                                      
            {
                push(vm, {Value_Type::NUMBER, -std::get<double>(a.data)});
            }
            else
            {
                vm_error("Invalid types for unary subtraction");
            })";
            break;
        }
        case OpCode::OP_MUL:
        {
            program += R"(
            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {
                push(vm, {Value_Type::NUMBER, std::get<double>(a.data) * std::get<double>(b.data)});
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
                push(vm, {Value_Type::STRING, result});
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
                push(vm, {Value_Type::STRING, result});
            }
            else
            {
                vm_error("Invalid types for multiplication");
            })";
            break;
        }
        case OpCode::OP_DIV:
        {
            program += R"(
            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {
                if (std::get<double>(b.data) == 0)                                                  
                {
                    vm_error("Division by zero");
                }
                push(vm, {Value_Type::NUMBER, std::get<double>(a.data) / std::get<double>(b.data)});
            }
            else
            {
                vm_error("Invalid types for division");
            })";
            break;
        }
        case OpCode::OP_MOD:
        {
            program += R"(
            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {
                if (std::get<double>(b.data) == 0)                                                  
                {
                    vm_error("Modulus by zero");
                }
                push(vm, {Value_Type::NUMBER, std::fmod(std::get<double>(a.data), std::get<double>(b.data))});
            }
            else
            {
                vm_error("Invalid types for modulus");
            })";
            break;
        }
        case OpCode::OP_EXP:
        {
            program += R"(
            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {
                push(vm, {Value_Type::NUMBER, std::pow(std::get<double>(a.data), std::get<double>(b.data))});
            }
            else
            {
                vm_error("Invalid types for power");
            })";
            break;
        }

        // Logical operations
        // uses type coercion, check VALUE_AS_BOOL for more info
        case OpCode::OP_AND:
        {
            program += R"(
            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            push(vm, {Value_Type::BOOL, VALUE_AS_BOOL(a) && VALUE_AS_BOOL(b)});)";
            break;
        }
        case OpCode::OP_OR:
        {
            program += R"(
            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            push(vm, {Value_Type::BOOL, VALUE_AS_BOOL(a) || VALUE_AS_BOOL(b)});)";
            break;
        }
        case OpCode::OP_NOT:
        {
            program += R"(
            Value a = pop(vm);                                                                       
            push(vm, {Value_Type::BOOL, !VALUE_AS_BOOL(a)});)";
            break;
        }

        // Comparison operations
        // uses type coercion, check VALUE_AS_BOOL, VALUE_AS_NUMBER, VALUE_AS_STRING for more info
        case OpCode::OP_EQ:
        {
            program += R"(
            // If both are strings, compare the strings
            // Else compare the bool values
            Value b = pop(vm);
            Value a = pop(vm);
            if(a.type == Value_Type::STRING && b.type == Value_Type::STRING){
                push(vm, {Value_Type::BOOL, VALUE_AS_STRING(a) == VALUE_AS_STRING(b)});
            }
            else if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){
                push(vm, {Value_Type::BOOL, VALUE_AS_NUMBER(a) == VALUE_AS_NUMBER(b)});
            }
            // Needs to be in this order 
            // If both are null, return true
            else if(a.type == Value_Type::NULL_VALUE && b.type == Value_Type::NULL_VALUE){
                push(vm, {Value_Type::BOOL, true});
            }
            // If one of them is null, return false
            else if(a.type == Value_Type::NULL_VALUE || b.type == Value_Type::NULL_VALUE){
                push(vm, {Value_Type::BOOL, false});
            }
            else{
                push(vm, {Value_Type::BOOL, VALUE_AS_BOOL(a) == VALUE_AS_BOOL(b)});
            })";
            break;
        }
        case OpCode::OP_NEQ:
        {
            program += R"(
            // If both are strings, compare the strings
            // Else compare the bool values
            Value b = pop(vm);
            Value a = pop(vm);
            if(a.type == Value_Type::STRING && b.type == Value_Type::STRING){
                push(vm, {Value_Type::BOOL, VALUE_AS_STRING(a) != VALUE_AS_STRING(b)});
            }
            else if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){
                push(vm, {Value_Type::BOOL, VALUE_AS_NUMBER(a) != VALUE_AS_NUMBER(b)});
            }
            // Needs to be in this order 
            // If both are null, return false        
            else if(a.type == Value_Type::NULL_VALUE && b.type == Value_Type::NULL_VALUE){
                push(vm, {Value_Type::BOOL, false});
            }
            // If one of them is null, return true
            else if(a.type == Value_Type::NULL_VALUE || b.type == Value_Type::NULL_VALUE){
                push(vm, {Value_Type::BOOL, true});
            }
            else{
                push(vm, {Value_Type::BOOL, VALUE_AS_BOOL(a) != VALUE_AS_BOOL(b)});
            })";
            break;
        }
        case OpCode::OP_GT:
        {
            program += R"(
            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {                                                                                      
                push(vm, {Value_Type::BOOL, std::get<double>(a.data) > std::get<double>(b.data)});     
            }                                                                                      
            else                                                                                   
            {                                                                                      
                vm_error("Invalid types for greater than comparison");                             
            })";
            break;
        }
        case OpCode::OP_GTEQ:
        {
            program += R"(
            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {                                                                                      
                push(vm, {Value_Type::BOOL, std::get<double>(a.data) >= std::get<double>(b.data)});    
            }                                                                                      
            else                                                                                   
            {                                                                                      
                vm_error("Invalid types for greater than or equal comparison");                    
            })";
            break;
        }
        case OpCode::OP_LT:
        {
            program += R"(
            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {                                                                                      
                push(vm, {Value_Type::BOOL, std::get<double>(a.data) < std::get<double>(b.data)});     
            }                                                                                      
            else                                                                                   
            {                                                                                      
                vm_error("Invalid types for less than comparison");                                
            })";
            break;
        }
        case OpCode::OP_LTEQ:
        {
            program += R"(
            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {                                                                                      
                push(vm, {Value_Type::BOOL, std::get<double>(a.data) <= std::get<double>(b.data)});    
            }                                                                                      
            else                                                                                   
            {                                                                                      
                vm_error("Invalid types for less than or equal comparison");                       
            })";
            break;
        }

        // Memory operations
        case OpCode::OP_LOAD:
        {
            program += R"(
            push(vm, get_vm_constant(vm, )" + std::to_string(func->code[++i]) + R"());)";
            break;
        }
        case OpCode::OP_STORE_VAR:
        {
            program += R"(
            set_variable(vm, vm->variable_names[)" + std::to_string(func->code[++i]) + R"(], pop(vm));)";
            break;
        }
        case OpCode::OP_UPDATE_VAR:
        {
            program += R"(
            update_variable(vm, vm->variable_names[)" + std::to_string(func->code[++i]) + R"(], pop(vm));)";
            break;
        }
        case OpCode::OP_LOAD_VAR:
        {
            program += R"(
            push(vm, get_variable(vm, vm->variable_names[)" + std::to_string(func->code[++i]) + R"(]));)";
            break;
        }
        case OpCode::OP_LOAD_FUNCTION_VAR:
        {
            program += R"(
            push(vm, get_function_variable(vm, vm->variable_names[)" + std::to_string(func->code[++i]) + R"(]));)";
            break;
        }

        // Array operations
        case OpCode::OP_CREATE_VECTOR:
        {
            program += R"(
            push(vm, {Value_Type::VECTOR, std::vector<Value>()});)";
            break;
        }
        case OpCode::OP_VECTOR_PUSH:
        {
            program += R"(
            Value value = pop(vm);
            Value vector = pop(vm);
            if (vector.type != Value_Type::VECTOR)
            {
                vm_error("Invalid type for vector push");
            }
            std::vector<Value> vec = VALUE_AS_VECTOR(vector);
            vec.push_back(value);
            push(vm, {Value_Type::VECTOR, vec});
            )";
            break;
        }
        case OpCode::OP_UPDATE_VECTOR_ELEMENT:
        {
            program += R"(
            Value index = pop(vm);                                                                    
            Value value = pop(vm);                                                                    
            Value vector = get_variable(vm, vm->variable_names[)" + std::to_string(func->code[++i]) + R"(]);)";
            program += R"(
            if (vector.type != Value_Type::VECTOR || index.type != Value_Type::NUMBER)                
            {                                                                                      
                vm_error("Invalid types for vector element access");                                 
            }                                                                                      
            std::vector<Value> vec = VALUE_AS_VECTOR(vector);                
            if (VALUE_AS_NUMBER(index) < 0 || VALUE_AS_NUMBER(index) >= vec.size())                  
            {                                                                                      
                vm_error("Index out of bounds - Vector element update");                                                    
            }                                                                                      
            vec[(int)VALUE_AS_NUMBER(index)] = value;                                               
            update_variable(vm, vm->variable_names[)" + std::to_string(func->code[i]) + R"(], {Value_Type::VECTOR, vec});)";
            break;
        }
        case OpCode::OP_LOAD_VECTOR_ELEMENT:
        {
            program += R"(
            Value index = pop(vm);                                                                    
            Value vector = get_variable(vm, vm->variable_names[)" + std::to_string(func->code[++i]) + R"(]);)";
            program += R"(
            if (vector.type != Value_Type::VECTOR || index.type != Value_Type::NUMBER)                
            {                                                                                      
                vm_error("Invalid types for vector element access");                                 
            }                                                 
            std::vector<Value> vec = VALUE_AS_VECTOR(vector);                                    
            if (VALUE_AS_NUMBER(index) < 0 || VALUE_AS_NUMBER(index) >= vec.size())
            {                                                                                      
                vm_error("Index out of bounds - Vector element access");                                                    
            }                                                                                                                         
            push(vm, vec[(int)VALUE_AS_NUMBER(index)]);                                                  
            )";
            break;
        }

        // Map operations

        case OpCode::OP_CREATE_MAP:
        {
            program += R"(
            push(vm, {Value_Type::MAP, std::map<std::string, Value>()});)";
            break;
        }
        case OpCode::OP_UPDATE_MAP_ELEMENT:
        {
            program += R"(
            Value value = pop(vm);                                                                    
            Value map_ = get_variable(vm, vm->variable_names[)" + std::to_string(func->code[i + 1]) + R"(]);)";
            program += R"(
            if (map_.type != Value_Type::MAP)                                                  
            {                                                                                      
                vm_error("Not a map");                                                           
            }                                                                                      
            std::map<std::string, Value> map_map = VALUE_AS_MAP(map_);                     
            map_map[vm->variable_names[)" + std::to_string(func->code[i + 2]) + R"(]] = value;        
            update_variable(vm, vm->variable_names[)" + std::to_string(func->code[i + 1]) + R"(], {Value_Type::MAP, map_map});)";
            i += 2;
            break;
        }
        case OpCode::OP_LOAD_MAP_ELEMENT:
        {
            program += R"(
            Value map_ = get_variable(vm, vm->variable_names[)" + std::to_string(func->code[++i]) + R"(]);)";
            program += R"(
            if (map_.type != Value_Type::MAP)                                                  
            {                                                                                      
                vm_error("Not a map");                                                           
            }                                                                                      
            std::map<std::string, Value> map_map = VALUE_AS_MAP(map_);                     
            push(vm, map_map[vm->variable_names[)" + std::to_string(func->code[++i]) + R"(]]);)";
            break;
        }

        // Access operations
        case OpCode::OP_ACCESS:
        {
            program += R"(
            Value index = pop(vm);
            Value obj = pop(vm);
            if (obj.type == Value_Type::MAP)
            {
                std::map<std::string, Value> map_map = VALUE_AS_MAP(obj);
                //check if the key exists
                if (map_map.find(VALUE_AS_STRING(index)) == map_map.end())
                {
                    vm_error("Key does not exist in map");
                }
                push(vm, map_map[VALUE_AS_STRING(index)]);
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
                push(vm, vec[(int)VALUE_AS_NUMBER(index)]);
            }
            else
            {
                vm_error("Invalid type for access");
            }
            )";
            break;
        }
        case OpCode::OP_ACCESS_FOR_UPDATE:
        {
            program += R"(
            Value index = pop(vm);
            Value obj = pop(vm);
            push(vm, obj);
            push(vm, index);

            if (obj.type == Value_Type::MAP)
            {
                std::map<std::string, Value> map_map = VALUE_AS_MAP(obj);
                //check if the key exists
                if (map_map.find(VALUE_AS_STRING(index)) == map_map.end())
                {
                    vm_error("Key does not exist in map");
                }
                push(vm, map_map[VALUE_AS_STRING(index)]);
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
                push(vm, vec[(int)VALUE_AS_NUMBER(index)]);
            }
            else
            {
                vm_error("Invalid type for access");
            }
            )";
            break;
        }
        case OpCode::OP_UPDATE_STACK_ELEMENT:
        {
            program += R"(
            Value value = pop(vm);
            Value index = pop(vm);
            Value obj = pop(vm);
        
            if (obj.type == Value_Type::MAP)
            {
                std::map<std::string, Value> map_map = VALUE_AS_MAP(obj);
                //if the key does not exist, it will be added
                map_map[VALUE_AS_STRING(index)] = value;
                push(vm, {Value_Type::MAP, map_map});
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
                push(vm, {Value_Type::VECTOR, vec});
            }
            else
            {
                vm_error("Invalid type for access");
            }
            )";
            break;
        }
        case OpCode::OP_ACCESS_STACK_ELEMENT_QUEUE:
        {
            program += R"(
            Value obj = pop(vm);
            int index = VALUE_AS_NUMBER(pop(vm));
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

                push(vm, {Value_Type::NUMBER, (double)(index + 1)});
                push(vm, {Value_Type::MAP, map_map});
                push(vm, value);
                push(vm, key);
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
                push(vm, {Value_Type::NUMBER, (double)(index + 1)}); // continue to use as the index
                push(vm, {Value_Type::VECTOR, vec});
                push(vm, value);
                push(vm, {Value_Type::NUMBER, (double)(index)}); // use as the key
            }
            else
            {
                vm_error("Invalid type for access");
            }
            )";
            break;
        }
        case OpCode::OP_NOT_EMPTY:
        {
            program += R"(
            Value obj = pop(vm);
            push(vm, obj);
            if (obj.type == Value_Type::MAP)
            {
                std::map<std::string, Value> map_map = VALUE_AS_MAP(obj);
                push(vm, {Value_Type::BOOL, map_map.size() > 0});
            }
            else if (obj.type == Value_Type::VECTOR)
            {
                std::vector<Value> vec = VALUE_AS_VECTOR(obj);
                push(vm, {Value_Type::BOOL, vec.size() > 0});
            }
            else
            {
                vm_error("Invalid type for not empty check");
            }
            )";
            break;
        }
        case OpCode::OP_POP:
        {
            program += R"(
            pop(vm);)";
            break;
        }

        // Control flow operations
        case OpCode::OP_RETURN:
        {
            program += R"(
            delete get_current_function_frame(vm);
            vm->function_frames.pop_back(); // return value is already on the stack
            return;)";
            break;
        }
        case OpCode::OP_JUMP:
        {
            program += R"(
            goto label_)" + std::to_string(func->code[++i] + 1) + R"(;)";
            break;
        }
        case OpCode::OP_JUMP_IF_FALSE:
        {
            program += R"(
            Value val = pop(vm);                                                                     
            if (!VALUE_AS_BOOL(val))                                                                
            {                                                                                      
                goto label_)" + std::to_string(func->code[++i] + 1) + R"(;                         
            })";
            break;
        }
        case OpCode::OP_FUNCTION_CALL:
        {
            program += R"(
            function* func = VALUE_AS_FUNCTION(pop(vm));)";
            program += R"(
            func->times_called++;)";
            program += R"(
            if(vm->jit && func->times_called == vm->calls_to_jit){ 
                jit_compile_function(vm, func);
            }

            if(vm->jit && func->jit_index != -1){
                vm->function_frames.push_back(create_function_frame(func));
                jit_run_function(vm, func->jit_index);
            }
            else{
                vm->function_frames.push_back(create_function_frame(func));

                get_current_function_frame(vm)->ip = func->code - 1; // -1 because the ip will be increased by 1

                run_vm();
            })";

            break;
        }
        case OpCode::OP_STD_LIB_CALL:
        {
            program += R"(
            STD_LIB_FUNCTION_INFO func = STD_LIB_FUNCTIONS_DEFINITIONS[)" + std::to_string(func->code[++i]) + R"(];)";
            program += R"(
            std::vector<std::any> args;
            for (int i = 0; i < (int)func.arg_types.size(); i++)                                      
            {                                                                                      
                int index = (int)func.arg_types.size() - i - 1;                                      
                if (!LII_type_matches_cpp_type(top(vm), func.arg_types[index]))                      
                {                                                                                  
                    vm_error("Invalid argument type. Expected: " + func.arg_types[index] + ", Got: " + get_value_type_string(top(vm))); 
                }                                                                                  
                args.push_back(cast_LII_type_to_cpp_type(pop(vm), func.arg_types[index]));          
            }
            std::reverse(args.begin(), args.end());)";
            program += R"(
            auto result = func.function(args, func.arg_types);)";
            program += R"(
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
                    push(vm, {Value_Type::NUMBER, (double)std::any_cast<int>(result)});
                }
                else if (func.return_type == "double")
                {
                    push(vm, {Value_Type::NUMBER, std::any_cast<double>(result)});
                }
                else if (func.return_type == "bool")
                {
                    push(vm, {Value_Type::BOOL, std::any_cast<bool>(result)});
                }
                else if (func.return_type == "std::string")
                {
                    push(vm, {Value_Type::STRING, std::any_cast<std::string>(result)});
                }
                else if (func.return_type == "std::vector<Value>")
                {
                    push(vm, {Value_Type::VECTOR, std::any_cast<std::vector<Value>>(result)});
                }
                else if (func.return_type == "Value")
                {
                    push(vm, std::any_cast<Value>(result));
                }
            })";
            break;
        }

        // Scope operations
        case OpCode::OP_INC_SCOPE:
        {
            program += R"(
            get_current_function_frame(vm)->current_scope++;                                          
            get_current_function_frame(vm)->variables.push_back(std::map<std::string, Value>());)";
            break;
        }
        case OpCode::OP_DEC_SCOPE:
        {
            program += R"(
            get_current_function_frame(vm)->current_scope--;
            get_current_function_frame(vm)->variables.pop_back();)";
            break;
        }

        // Output
        case OpCode::OP_PRINT:
        {
            program += R"(
            print_value(pop(vm));
            std::cout << std::endl;)";
            break;
        }

        default:
            std::cout << "Unknown opcode: " << (int)func->code[i] << std::endl;
            exit(1);
        }

        program += "\n}\n";
    }
    program += "}\n";

    std::ofstream out("./jit_functions/" + jit_name + ".cpp");
    out << program;
    out.close();

    // Compile the program
    std::string command = "clang++-16 ";
    command += JIT_OPTIMIZATION_LEVEL;
    command += " -shared -fPIC -o ./jit_functions/" + jit_name + ".so " + "./jit_functions/" + jit_name + ".cpp";
    system(command.c_str());

    // Load the shared library
    void* handle = dlopen(("./jit_functions/" + jit_name + ".so").c_str(), RTLD_LAZY);
    if(!handle){
        std::cout << "Failed to load shared library: " << dlerror() << std::endl;
        exit(1);
    }

    // Get the function pointer
    JIT_FUNCTION jit_func = (JIT_FUNCTION)dlsym(handle, jit_name.c_str());
    if(!jit_func){
        std::cout << "Failed to get function pointer: " << dlerror() << std::endl;
        exit(1);
    }

    jit_add_function(vm, jit_func);
    func->jit_index = vm->jit_functions.size() - 1;
}


#endif // JIT_HPP