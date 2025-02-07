
                        #include <iostream>
                        #include "../src_bytecode/VM.hpp"
                        #include "../src_bytecode/Value.hpp"
                        #include "../src_bytecode/std_lib/std_lib.hpp"
                        #include "../src_bytecode/virtual_machine.hpp"
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

label_7: 
// OP_RETURN
{

            delete get_current_function_frame(vm);
            vm->function_frames.pop_back(); // return value is already on the stack
            return;
}
}
