
                        #include <iostream>
                        #include "../src_bytecode/VM.hpp"
                        #include "../src_bytecode/Value.hpp"
                        extern "C" void jit_0(VM* vm){
label_0: 
{

            set_variable(vm, vm->variable_names[1], pop(vm));
}

label_2: 
{

            get_current_function_frame(vm)->current_scope++;                                          
            get_current_function_frame(vm)->variables.push_back(std::map<std::string, Value>());
}

label_3: 
{

            push(vm, get_vm_constant(vm, 1));
}

label_5: 
{

            set_variable(vm, vm->variable_names[2], pop(vm));
}

label_7: 
{

            push(vm, get_vm_constant(vm, 2));
}

label_9: 
{

            push(vm, get_variable(vm, vm->variable_names[2]));
}

label_11: 
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

label_12: 
{

            Value val = pop(vm);                                                                     
            if (!VALUE_AS_BOOL(val))                                                                
            {                                                                                      
                goto label_26;                         
            }
}

label_14: 
{

            push(vm, get_variable(vm, vm->variable_names[1]));
}

label_16: 
{

            print_value(pop(vm));
            std::cout << std::endl;
}

label_17: 
{

            push(vm, get_vm_constant(vm, 3));
}

label_19: 
{

            push(vm, get_variable(vm, vm->variable_names[2]));
}

label_21: 
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

label_22: 
{

            update_variable(vm, vm->variable_names[2], pop(vm));
}

label_24: 
{

            goto label_7;
}

label_26: 
{

            get_current_function_frame(vm)->current_scope--;
            get_current_function_frame(vm)->variables.pop_back();
}

label_27: 
{

            push(vm, get_variable(vm, vm->variable_names[1]));
}

label_29: 
{

            delete get_current_function_frame(vm);
            vm->function_frames.pop_back(); // return value is already on the stack
            return;
}
}
