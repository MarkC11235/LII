
                        #include <iostream>
                        #include "../src_bytecode/VM.hpp"
                        #include "../src_bytecode/Value.hpp"
                        #include "../src_bytecode/std_lib/std_lib.hpp"
                        extern "C" void jit_1(VM* vm){
label_0: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[2], pop(vm));
}

label_2: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[2]));
}

label_4: 
// OP_STD_LIB_CALL
{

            STD_LIB_FUNCTION_INFO func = STD_LIB_FUNCTIONS_DEFINITIONS[12];
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

label_6: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[5], pop(vm));
}

label_8: 
// OP_INC_SCOPE
{

            get_current_function_frame(vm)->current_scope++;                                          
            get_current_function_frame(vm)->variables.push_back(std::map<std::string, Value>());
}

label_9: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 6));
}

label_11: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[3], pop(vm));
}

label_13: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[5]));
}

label_15: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[3]));
}

label_17: 
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

label_18: 
// OP_JUMP_IF_FALSE
{

            Value val = pop(vm);                                                                     
            if (!VALUE_AS_BOOL(val))                                                                
            {                                                                                      
                goto label_87;                         
            }
}

label_20: 
// OP_INC_SCOPE
{

            get_current_function_frame(vm)->current_scope++;                                          
            get_current_function_frame(vm)->variables.push_back(std::map<std::string, Value>());
}

label_21: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 7));
}

label_23: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[3]));
}

label_25: 
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

label_26: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[6], pop(vm));
}

label_28: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[5]));
}

label_30: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[6]));
}

label_32: 
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

label_33: 
// OP_JUMP_IF_FALSE
{

            Value val = pop(vm);                                                                     
            if (!VALUE_AS_BOOL(val))                                                                
            {                                                                                      
                goto label_77;                         
            }
}

label_35: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[6]));
}

label_37: 
// OP_LOAD_VECTOR_ELEMENT
{

            Value index = pop(vm);                                                                    
            Value vector = get_variable(vm, vm->variable_names[2]);
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
            
}

label_39: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[3]));
}

label_41: 
// OP_LOAD_VECTOR_ELEMENT
{

            Value index = pop(vm);                                                                    
            Value vector = get_variable(vm, vm->variable_names[2]);
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
            
}

label_43: 
// OP_GT
{

            Value a = pop(vm);                                                                       
            Value b = pop(vm);                                                                        
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {                                                                                      
                push(vm, {Value_Type::BOOL, std::get<double>(a.data) > std::get<double>(b.data)});     
            }                                                                                      
            else                                                                                   
            {                                                                                      
                vm_error("Invalid types for greater than comparison");                             
            }
}

label_44: 
// OP_JUMP_IF_FALSE
{

            Value val = pop(vm);                                                                     
            if (!VALUE_AS_BOOL(val))                                                                
            {                                                                                      
                goto label_68;                         
            }
}

label_46: 
// OP_INC_SCOPE
{

            get_current_function_frame(vm)->current_scope++;                                          
            get_current_function_frame(vm)->variables.push_back(std::map<std::string, Value>());
}

label_47: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[3]));
}

label_49: 
// OP_LOAD_VECTOR_ELEMENT
{

            Value index = pop(vm);                                                                    
            Value vector = get_variable(vm, vm->variable_names[2]);
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
            
}

label_51: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[7], pop(vm));
}

label_53: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[6]));
}

label_55: 
// OP_LOAD_VECTOR_ELEMENT
{

            Value index = pop(vm);                                                                    
            Value vector = get_variable(vm, vm->variable_names[2]);
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
            
}

label_57: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[3]));
}

label_59: 
// OP_UPDATE_VECTOR_ELEMENT
{

            Value index = pop(vm);                                                                    
            Value value = pop(vm);                                                                    
            Value vector = get_variable(vm, vm->variable_names[2]);
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
            update_variable(vm, vm->variable_names[2], {Value_Type::VECTOR, vec});
}

label_61: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[7]));
}

label_63: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[6]));
}

label_65: 
// OP_UPDATE_VECTOR_ELEMENT
{

            Value index = pop(vm);                                                                    
            Value value = pop(vm);                                                                    
            Value vector = get_variable(vm, vm->variable_names[2]);
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
            update_variable(vm, vm->variable_names[2], {Value_Type::VECTOR, vec});
}

label_67: 
// OP_DEC_SCOPE
{

            get_current_function_frame(vm)->current_scope--;
            get_current_function_frame(vm)->variables.pop_back();
}

label_68: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 8));
}

label_70: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[6]));
}

label_72: 
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

label_73: 
// OP_UPDATE_VAR
{

            update_variable(vm, vm->variable_names[6], pop(vm));
}

label_75: 
// OP_JUMP
{

            goto label_28;
}

label_77: 
// OP_DEC_SCOPE
{

            get_current_function_frame(vm)->current_scope--;
            get_current_function_frame(vm)->variables.pop_back();
}

label_78: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 9));
}

label_80: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[3]));
}

label_82: 
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

label_83: 
// OP_UPDATE_VAR
{

            update_variable(vm, vm->variable_names[3], pop(vm));
}

label_85: 
// OP_JUMP
{

            goto label_13;
}

label_87: 
// OP_DEC_SCOPE
{

            get_current_function_frame(vm)->current_scope--;
            get_current_function_frame(vm)->variables.pop_back();
}

label_88: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[2]));
}

label_90: 
// OP_RETURN
{

            delete get_current_function_frame(vm);
            vm->function_frames.pop_back(); // return value is already on the stack
            return;
}
}
