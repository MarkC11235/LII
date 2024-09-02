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

typedef void (*JIT_FUNCTION)(VM* vm);

std::vector<JIT_FUNCTION> jit_functions;

void jit_add_function(JIT_FUNCTION func)
{
    jit_functions.push_back(func);
}

void jit_run_function(VM* vm, int index)
{
    jit_functions[index](vm);
}

void jit_compile_function(function* func)
{
    std::string jit_name = "jit_" + std::to_string(jit_functions.size());
    std::string program = R"(
                        #include <iostream>
                        #include "../src_bytecode/VM.hpp"
                        #include "../src_bytecode/Value.hpp"
                        extern "C" void )" + jit_name + R"((VM* vm){)";

    for(int i = 0; i < func->count; i++){
        program += "\nlabel_" + std::to_string(i) + ": \n";
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
            Value value = pop(vm);                                                                    
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
                vm_error("Index out of bounds");                                                    
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
            if (VALUE_AS_NUMBER(index) < 0 || VALUE_AS_NUMBER(index) >= VALUE_AS_VECTOR(vector).size())
            {                                                                                      
                vm_error("Index out of bounds");                                                    
            }                                                                                      
            std::vector<Value> vec = VALUE_AS_VECTOR(vector);                                        
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

        //leave out nested functions and std lib calls for now

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

    jit_add_function(jit_func);
    func->jit_index = jit_functions.size() - 1;
}


#endif // JIT_HPP