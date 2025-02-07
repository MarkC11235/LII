
                        #include <iostream>
                        #include "../src_bytecode/VM.hpp"
                        #include "../src_bytecode/Value.hpp"
                        #include "../src_bytecode/std_lib/std_lib.hpp"
                        #include "../src_bytecode/virtual_machine.hpp"
                        #include "../src_bytecode/bytecode_generator.hpp"
                        #include "../src_bytecode/jit.hpp"
                        extern "C" void jit_0(VM* vm){
label_0: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[3], pop(vm));
}

label_2: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[3]));
}

label_4: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 1));
}

label_6: 
// OP_LT
{

            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {                                                                                      
                push(vm, {Value_Type::BOOL, std::get<double>(a.data) < std::get<double>(b.data)});     
            }                                                                                      
            else                                                                                   
            {                                                                                      
                vm_error("Invalid types for less than comparison");                                
            }
}

label_7: 
// OP_JUMP_IF_FALSE
{

            Value val = pop(vm);                                                                     
            if (!VALUE_AS_BOOL(val))                                                                
            {                                                                                      
                goto label_16;                         
            }
}

label_9: 
// OP_INC_SCOPE
{

            get_current_function_frame(vm)->current_scope++;                                          
            get_current_function_frame(vm)->variables.push_back(std::map<std::string, Value>());
}

label_10: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 2));
}

label_12: 
// OP_RETURN
{

            delete get_current_function_frame(vm);
            vm->function_frames.pop_back(); // return value is already on the stack
            return;
}

label_13: 
// OP_DEC_SCOPE
{

            get_current_function_frame(vm)->current_scope--;
            get_current_function_frame(vm)->variables.pop_back();
}

label_14: 
// OP_JUMP
{

            goto label_16;
}

label_16: 
// OP_INC_SCOPE
{

            get_current_function_frame(vm)->current_scope++;                                          
            get_current_function_frame(vm)->variables.push_back(std::map<std::string, Value>());
}

label_17: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 3));
}

label_19: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[4], pop(vm));
}

label_21: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[4]));
}

label_23: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[4]));
}

label_25: 
// OP_MUL
{

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
            }
}

label_26: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[3]));
}

label_28: 
// OP_LTEQ
{

            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {                                                                                      
                push(vm, {Value_Type::BOOL, std::get<double>(a.data) <= std::get<double>(b.data)});    
            }                                                                                      
            else                                                                                   
            {                                                                                      
                vm_error("Invalid types for less than or equal comparison");                       
            }
}

label_29: 
// OP_JUMP_IF_FALSE
{

            Value val = pop(vm);                                                                     
            if (!VALUE_AS_BOOL(val))                                                                
            {                                                                                      
                goto label_57;                         
            }
}

label_31: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[3]));
}

label_33: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[4]));
}

label_35: 
// OP_MOD
{

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
            }
}

label_36: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 4));
}

label_38: 
// OP_EQ
{

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
            }
}

label_39: 
// OP_JUMP_IF_FALSE
{

            Value val = pop(vm);                                                                     
            if (!VALUE_AS_BOOL(val))                                                                
            {                                                                                      
                goto label_48;                         
            }
}

label_41: 
// OP_INC_SCOPE
{

            get_current_function_frame(vm)->current_scope++;                                          
            get_current_function_frame(vm)->variables.push_back(std::map<std::string, Value>());
}

label_42: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 5));
}

label_44: 
// OP_RETURN
{

            delete get_current_function_frame(vm);
            vm->function_frames.pop_back(); // return value is already on the stack
            return;
}

label_45: 
// OP_DEC_SCOPE
{

            get_current_function_frame(vm)->current_scope--;
            get_current_function_frame(vm)->variables.pop_back();
}

label_46: 
// OP_JUMP
{

            goto label_48;
}

label_48: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[4]));
}

label_50: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 6));
}

label_52: 
// OP_ADD
{

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
            }
}

label_53: 
// OP_UPDATE_VAR
{

            update_variable(vm, vm->variable_names[4], pop(vm));
}

label_55: 
// OP_JUMP
{

            goto label_21;
}

label_57: 
// OP_DEC_SCOPE
{

            get_current_function_frame(vm)->current_scope--;
            get_current_function_frame(vm)->variables.pop_back();
}

label_58: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 7));
}

label_60: 
// OP_RETURN
{

            delete get_current_function_frame(vm);
            vm->function_frames.pop_back(); // return value is already on the stack
            return;
}
}
