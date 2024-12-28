
                        #include <iostream>
                        #include "../src_bytecode/VM.hpp"
                        #include "../src_bytecode/Value.hpp"
                        #include "../src_bytecode/std_lib/std_lib.hpp"
                        #include "../src_bytecode/virtual_machine.hpp"
                        #include "../src_bytecode/jit.hpp"
                        extern "C" void jit_2(VM* vm){
label_0: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[2], pop(vm));
}

label_2: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[0], pop(vm));
}

label_4: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[2]));
}

label_6: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 15));
}

label_8: 
// OP_ACCESS
{

            Value index = pop(vm);
            Value obj = pop(vm);
            if (obj.type == Value_Type::STRUCT)
            {
                std::map<std::string, Value> struct_map = VALUE_AS_STRUCT(obj);
                //check if the key exists
                if (struct_map.find(VALUE_AS_STRING(index)) == struct_map.end())
                {
                    vm_error("Key does not exist in struct");
                }
                push(vm, struct_map[VALUE_AS_STRING(index)]);
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
            
}

label_9: 
// OP_PRINT
{

            print_value(pop(vm));
            std::cout << std::endl;
}

label_10: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 16));
}

label_12: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[2]));
}

label_14: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 17));
}

label_16: 
// OP_ACCESS
{

            Value index = pop(vm);
            Value obj = pop(vm);
            if (obj.type == Value_Type::STRUCT)
            {
                std::map<std::string, Value> struct_map = VALUE_AS_STRUCT(obj);
                //check if the key exists
                if (struct_map.find(VALUE_AS_STRING(index)) == struct_map.end())
                {
                    vm_error("Key does not exist in struct");
                }
                push(vm, struct_map[VALUE_AS_STRING(index)]);
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
            
}

label_17: 
// OP_NEQ
{

            // If both are strings, compare the strings
            // Else compare the bool values
            Value a = pop(vm);
            Value b = pop(vm);
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
            }
}

label_18: 
// OP_JUMP_IF_FALSE
{

            Value val = pop(vm);                                                                     
            if (!VALUE_AS_BOOL(val))                                                                
            {                                                                                      
                goto label_36;                         
            }
}

label_20: 
// OP_INC_SCOPE
{

            get_current_function_frame(vm)->current_scope++;                                          
            get_current_function_frame(vm)->variables.push_back(std::map<std::string, Value>());
}

label_21: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[0]));
}

label_23: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[2]));
}

label_25: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 18));
}

label_27: 
// OP_ACCESS
{

            Value index = pop(vm);
            Value obj = pop(vm);
            if (obj.type == Value_Type::STRUCT)
            {
                std::map<std::string, Value> struct_map = VALUE_AS_STRUCT(obj);
                //check if the key exists
                if (struct_map.find(VALUE_AS_STRING(index)) == struct_map.end())
                {
                    vm_error("Key does not exist in struct");
                }
                push(vm, struct_map[VALUE_AS_STRING(index)]);
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
            
}

label_28: 
// OP_LOAD_FUNCTION_VAR
{

            push(vm, get_function_variable(vm, vm->variable_names[6]));
}

label_30: 
// OP_FUNCTION_CALL
{

            function* func = VALUE_AS_FUNCTION(pop(vm));
            func->times_called++;
            if(vm->jit && func->times_called == CALLS_TO_JIT){ 
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
            }
}

label_31: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[7], pop(vm));
}

label_33: 
// OP_DEC_SCOPE
{

            get_current_function_frame(vm)->current_scope--;
            get_current_function_frame(vm)->variables.pop_back();
}

label_34: 
// OP_JUMP
{

            goto label_36;
}

label_36: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 19));
}

label_38: 
// OP_RETURN
{

            delete get_current_function_frame(vm);
            vm->function_frames.pop_back(); // return value is already on the stack
            return;
}
}
