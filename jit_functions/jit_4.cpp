
                        #include <iostream>
                        #include "../src_bytecode/VM.hpp"
                        #include "../src_bytecode/Value.hpp"
                        #include "../src_bytecode/std_lib/std_lib.hpp"
                        #include "../src_bytecode/virtual_machine.hpp"
                        #include "../src_bytecode/jit.hpp"
                        extern "C" void jit_4(VM* vm){
label_0: 
// OP_CREATE_VECTOR
{

            push(vm, {Value_Type::VECTOR, std::vector<Value>()});
}

label_1: 
// OP_CREATE_VECTOR
{

            push(vm, {Value_Type::VECTOR, std::vector<Value>()});
}

label_2: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 17));
}

label_4: 
// OP_VECTOR_PUSH
{

            Value value = pop(vm);
            Value vector = pop(vm);
            if (vector.type != Value_Type::VECTOR)
            {
                vm_error("Invalid type for vector push");
            }
            std::vector<Value> vec = VALUE_AS_VECTOR(vector);
            vec.push_back(value);
            push(vm, {Value_Type::VECTOR, vec});
            
}

label_5: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 18));
}

label_7: 
// OP_VECTOR_PUSH
{

            Value value = pop(vm);
            Value vector = pop(vm);
            if (vector.type != Value_Type::VECTOR)
            {
                vm_error("Invalid type for vector push");
            }
            std::vector<Value> vec = VALUE_AS_VECTOR(vector);
            vec.push_back(value);
            push(vm, {Value_Type::VECTOR, vec});
            
}

label_8: 
// OP_VECTOR_PUSH
{

            Value value = pop(vm);
            Value vector = pop(vm);
            if (vector.type != Value_Type::VECTOR)
            {
                vm_error("Invalid type for vector push");
            }
            std::vector<Value> vec = VALUE_AS_VECTOR(vector);
            vec.push_back(value);
            push(vm, {Value_Type::VECTOR, vec});
            
}

label_9: 
// OP_CREATE_VECTOR
{

            push(vm, {Value_Type::VECTOR, std::vector<Value>()});
}

label_10: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 19));
}

label_12: 
// OP_VECTOR_PUSH
{

            Value value = pop(vm);
            Value vector = pop(vm);
            if (vector.type != Value_Type::VECTOR)
            {
                vm_error("Invalid type for vector push");
            }
            std::vector<Value> vec = VALUE_AS_VECTOR(vector);
            vec.push_back(value);
            push(vm, {Value_Type::VECTOR, vec});
            
}

label_13: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 20));
}

label_15: 
// OP_VECTOR_PUSH
{

            Value value = pop(vm);
            Value vector = pop(vm);
            if (vector.type != Value_Type::VECTOR)
            {
                vm_error("Invalid type for vector push");
            }
            std::vector<Value> vec = VALUE_AS_VECTOR(vector);
            vec.push_back(value);
            push(vm, {Value_Type::VECTOR, vec});
            
}

label_16: 
// OP_VECTOR_PUSH
{

            Value value = pop(vm);
            Value vector = pop(vm);
            if (vector.type != Value_Type::VECTOR)
            {
                vm_error("Invalid type for vector push");
            }
            std::vector<Value> vec = VALUE_AS_VECTOR(vector);
            vec.push_back(value);
            push(vm, {Value_Type::VECTOR, vec});
            
}

label_17: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[10], pop(vm));
}

label_19: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[10]));
}

label_21: 
// OP_RETURN
{

            delete get_current_function_frame(vm);
            vm->function_frames.pop_back(); // return value is already on the stack
            return;
}
}
