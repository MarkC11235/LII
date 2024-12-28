
                        #include <iostream>
                        #include "../src_bytecode/VM.hpp"
                        #include "../src_bytecode/Value.hpp"
                        #include "../src_bytecode/std_lib/std_lib.hpp"
                        #include "../src_bytecode/virtual_machine.hpp"
                        #include "../src_bytecode/jit.hpp"
                        extern "C" void jit_1(VM* vm){
label_0: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[3], pop(vm));
}

label_2: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[2], pop(vm));
}

label_4: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[0], pop(vm));
}

label_6: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[0]));
}

label_8: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[4], pop(vm));
}

label_10: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[4]));
}

label_12: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 12));
}

label_14: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[3]));
}

label_16: 
// OP_UPDATE_STACK_ELEMENT
{

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
            
}

label_17: 
// OP_UPDATE_VAR
{

            update_variable(vm, vm->variable_names[4], pop(vm));
}

label_19: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[4]));
}

label_21: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 13));
}

label_23: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[2]));
}

label_25: 
// OP_UPDATE_STACK_ELEMENT
{

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
            
}

label_26: 
// OP_UPDATE_VAR
{

            update_variable(vm, vm->variable_names[4], pop(vm));
}

label_28: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[4]));
}

label_30: 
// OP_RETURN
{

            delete get_current_function_frame(vm);
            vm->function_frames.pop_back(); // return value is already on the stack
            return;
}
}
