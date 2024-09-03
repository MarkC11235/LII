#ifndef JIT_HPP
#define JIT_HPP

#include <vector>
#include <iostream>
#include <dlfcn.h> // For dlopen, dlsym, dlclose
#include <unistd.h>
#include <limits.h>

#include "VM.hpp"
#include "Value.hpp"
#include "opcodes.hpp"

void jit_add_function(VM* vm, JIT_FUNCTION func)
{
    vm->jit_functions.push_back(func);
}

void jit_run_function(VM* vm, int index)
{
    vm->jit_functions[index](vm);
}

void jit_compile_function(VM* vm, function* func)
{
    std::string jit_name = "jit_" + std::to_string(vm->jit_functions.size());
    std::string program = R"(
                        #include <iostream>
                        #include "../src_bytecode/VM.hpp"
                        #include "../src_bytecode/Value.hpp"
                        #include "../src_bytecode/std_lib/std_lib.hpp"
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
            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {
                push(vm, {Value_Type::NUMBER, std::get<double>(a.data) + std::get<double>(b.data)});
            }
            else if (a.type == Value_Type::STRING || b.type == Value_Type::STRING)
            {
                push(vm, {Value_Type::STRING, VALUE_AS_STRING(a) + VALUE_AS_STRING(b)});
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
            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
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
            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {
                push(vm, {Value_Type::NUMBER, std::get<double>(a.data) * std::get<double>(b.data)});
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
            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
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
            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
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

        // Logical operations
        // uses type coercion, check VALUE_AS_BOOL for more info
        case OpCode::OP_AND:
        {
            program += R"(
            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
            push(vm, {Value_Type::BOOL, VALUE_AS_BOOL(a) && VALUE_AS_BOOL(b)});)";
            break;
        }
        case OpCode::OP_OR:
        {
            program += R"(
            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
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
            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
            if(a.type == Value_Type::STRING && b.type == Value_Type::STRING){                       
                push(vm, {Value_Type::BOOL, VALUE_AS_STRING(a) == VALUE_AS_STRING(b)});                 
            }                                                                                      
            else if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){                 
                push(vm, {Value_Type::BOOL, VALUE_AS_NUMBER(a) == VALUE_AS_NUMBER(b)});                 
            }                                                                                      
            else{                                                                                  
                push(vm, {Value_Type::BOOL, VALUE_AS_BOOL(a) == VALUE_AS_BOOL(b)});                    
            })";
            break;
        }
        case OpCode::OP_NEQ:
        {
            program += R"(
            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
            if(a.type == Value_Type::STRING && b.type == Value_Type::STRING){                       
                push(vm, {Value_Type::BOOL, VALUE_AS_STRING(a) != VALUE_AS_STRING(b)});                 
            }                                                                                      
            else if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){                 
                push(vm, {Value_Type::BOOL, VALUE_AS_NUMBER(a) != VALUE_AS_NUMBER(b)});                 
            }                                                                                      
            else{                                                                                  
                push(vm, {Value_Type::BOOL, VALUE_AS_BOOL(a) != VALUE_AS_BOOL(b)});                    
            })";
            break;
        }
        case OpCode::OP_GT:
        {
            program += R"(
            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
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
            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
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
            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
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
            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
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
            push(vm, get_function_variable(vm, vm->variable_names[)" + std::to_string(func->code[++i]) + R"(]);)";
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
            Value index = pop(vm);                                                                    
            Value vector = get_variable(vm, vm->variable_names[)" + std::to_string(func->code[++i]) + R"(]);)";
            program += R"(
            if (vector.type != Value_Type::VECTOR || index.type != Value_Type::NUMBER)                
            {                                                                                      
                vm_error("Invalid types for vector push back");                                     
            }                                                                                      
            std::vector<Value> vec = VALUE_AS_VECTOR(vector);                                        
            vec.push_back(value);                                                                   
            update_variable(vm, vm->variable_names[)" + std::to_string(func->code[i]) + R"(], {Value_Type::VECTOR, vec});)";
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

        // Struct operations

        case OpCode::OP_CREATE_STRUCT:
        {
            program += R"(
            push(vm, {Value_Type::STRUCT, std::map<std::string, Value>()});)";
            break;
        }
        case OpCode::OP_UPDATE_STRUCT_ELEMENT:
        {
            program += R"(
            Value value = pop(vm);                                                                    
            Value struct_ = get_variable(vm, vm->variable_names[)" + std::to_string(func->code[i + 1]) + R"(]);)";
            program += R"(
            if (struct_.type != Value_Type::STRUCT)                                                  
            {                                                                                      
                vm_error("Not a struct");                                                           
            }                                                                                      
            std::map<std::string, Value> struct_map = VALUE_AS_STRUCT(struct_);                     
            struct_map[vm->variable_names[)" + std::to_string(func->code[i + 2]) + R"(]] = value;        
            update_variable(vm, vm->variable_names[)" + std::to_string(func->code[i + 1]) + R"(], {Value_Type::STRUCT, struct_map});)";
            i += 2;
            break;
        }
        case OpCode::OP_LOAD_STRUCT_ELEMENT:
        {
            program += R"(
            Value struct_ = get_variable(vm, vm->variable_names[)" + std::to_string(func->code[++i]) + R"(]);)";
            program += R"(
            if (struct_.type != Value_Type::STRUCT)                                                  
            {                                                                                      
                vm_error("Not a struct");                                                           
            }                                                                                      
            std::map<std::string, Value> struct_map = VALUE_AS_STRUCT(struct_);                     
            push(vm, struct_map[vm->variable_names[)" + std::to_string(func->code[++i]) + R"(]]);)";
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
            vm_error("Function calls are not supported in JIT");
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
    std::string command = "clang++-16 -shared -fPIC -o ./jit_functions/" + jit_name + ".so " + "./jit_functions/" + jit_name + ".cpp";
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