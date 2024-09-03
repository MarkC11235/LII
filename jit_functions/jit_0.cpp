
                        #include <iostream>
                        #include "../src_bytecode/VM.hpp"
                        #include "../src_bytecode/Value.hpp"
                        #include "../src_bytecode/std_lib/std_lib.hpp"
                        extern "C" void jit_0(VM* vm){
label_0: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[1], pop(vm));
}

label_2: 
// OP_CREATE_VECTOR
{

            push(vm, {Value_Type::VECTOR, std::vector<Value>()});
}

label_3: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[2], pop(vm));
}

label_5: 
// OP_INC_SCOPE
{

            get_current_function_frame(vm)->current_scope++;                                          
            get_current_function_frame(vm)->variables.push_back(std::map<std::string, Value>());
}

label_6: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 1));
}

label_8: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[3], pop(vm));
}

label_10: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[1]));
}

label_12: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[3]));
}

label_14: 
// OP_LT
{

            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {                                                                                      
                push(vm, {Value_Type::BOOL, std::get<double>(a.data) < std::get<double>(b.data)});     
            }                                                                                      
            else                                                                                   
            {                                                                                      
                vm_error("Invalid types for less than comparison");                                
            }
}

label_15: 
// OP_JUMP_IF_FALSE
{

            Value val = pop(vm);                                                                     
            if (!VALUE_AS_BOOL(val))                                                                
            {                                                                                      
                goto label_38;                         
            }
}

label_17: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[2]));
}

label_19: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 2));
}

label_21: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 3));
}

label_23: 
// OP_STD_LIB_CALL
{

            STD_LIB_FUNCTION_INFO func = STD_LIB_FUNCTIONS_DEFINITIONS[26];
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
            std::reverse(args.begin(), args.end());
            auto result = func.function(args, func.arg_types);
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
            }
}

label_25: 
// OP_STD_LIB_CALL
{

            STD_LIB_FUNCTION_INFO func = STD_LIB_FUNCTIONS_DEFINITIONS[13];
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
            std::reverse(args.begin(), args.end());
            auto result = func.function(args, func.arg_types);
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
            }
}

label_27: 
// OP_UPDATE_VAR
{

            update_variable(vm, vm->variable_names[2], pop(vm));
}

label_29: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 4));
}

label_31: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[3]));
}

label_33: 
// OP_ADD
{

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
            }
}

label_34: 
// OP_UPDATE_VAR
{

            update_variable(vm, vm->variable_names[3], pop(vm));
}

label_36: 
// OP_JUMP
{

            goto label_10;
}

label_38: 
// OP_DEC_SCOPE
{

            get_current_function_frame(vm)->current_scope--;
            get_current_function_frame(vm)->variables.pop_back();
}

label_39: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[2]));
}

label_41: 
// OP_RETURN
{

            delete get_current_function_frame(vm);
            vm->function_frames.pop_back(); // return value is already on the stack
            return;
}
}
