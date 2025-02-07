
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

            set_variable(vm, vm->variable_names[4], pop(vm));
}

label_2: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[2], pop(vm));
}

label_4: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[8], pop(vm));
}

label_6: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[4]));
}

label_8: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 15));
}

label_10: 
// OP_ACCESS
{

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
            
}

label_11: 
// OP_PRINT
{

            print_value(pop(vm));
            std::cout << std::endl;
}

label_12: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[4]));
}

label_14: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 16));
}

label_16: 
// OP_ACCESS
{

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
            
}

label_17: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 17));
}

label_19: 
// OP_NEQ
{

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
            }
}

label_20: 
// OP_JUMP_IF_FALSE
{

            Value val = pop(vm);                                                                     
            if (!VALUE_AS_BOOL(val))                                                                
            {                                                                                      
                goto label_40;                         
            }
}

label_22: 
// OP_INC_SCOPE
{

            get_current_function_frame(vm)->current_scope++;                                          
            get_current_function_frame(vm)->variables.push_back(std::map<std::string, Value>());
}

label_23: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[8]));
}

label_25: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[2]));
}

label_27: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[4]));
}

label_29: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 18));
}

label_31: 
// OP_ACCESS
{

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
            
}

label_32: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[8]));
}

label_34: 
// OP_FUNCTION_CALL
{

            function* func = VALUE_AS_FUNCTION(pop(vm));
            func->times_called++;
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
            }
}

label_35: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[9], pop(vm));
}

label_37: 
// OP_DEC_SCOPE
{

            get_current_function_frame(vm)->current_scope--;
            get_current_function_frame(vm)->variables.pop_back();
}

label_38: 
// OP_JUMP
{

            goto label_40;
}

label_40: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 19));
}

label_42: 
// OP_RETURN
{

            delete get_current_function_frame(vm);
            vm->function_frames.pop_back(); // return value is already on the stack
            return;
}
}
