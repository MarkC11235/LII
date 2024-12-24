
                        #include <iostream>
                        #include "../src_bytecode/VM.hpp"
                        #include "../src_bytecode/Value.hpp"
                        #include "../src_bytecode/std_lib/std_lib.hpp"
                        #include "../src_bytecode/virtual_machine.hpp"
                        #include "../src_bytecode/jit.hpp"
                        extern "C" void jit_3(VM* vm){
label_0: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[7], pop(vm));
}

label_2: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[6], pop(vm));
}

label_4: 
// OP_STORE_VAR
{

            set_variable(vm, vm->variable_names[5], pop(vm));
}

label_6: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[6]));
}

label_8: 
// OP_LOAD_VAR
{

            push(vm, get_variable(vm, vm->variable_names[7]));
}

label_10: 
// OP_LOAD_FUNCTION_VAR
{

            push(vm, get_function_variable(vm, vm->variable_names[5]));
}

label_12: 
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

label_13: 
// OP_RETURN
{

            delete get_current_function_frame(vm);
            vm->function_frames.pop_back(); // return value is already on the stack
            return;
}
}
