
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

            set_variable(vm, vm->variable_names[5], pop(vm));
}

label_2: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[5]));
}

label_4: 
// OP_LOAD
{

            push(vm, get_vm_constant(vm, 6));
}

label_6: 
// OP_EXP
{

            Value b = pop(vm);                                                                        
            Value a = pop(vm);                                                                       
            if (a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)                       
            {
                push(vm, {Value_Type::NUMBER, std::pow(std::get<double>(a.data), std::get<double>(b.data))});
            }
            else
            {
                vm_error("Invalid types for power");
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
