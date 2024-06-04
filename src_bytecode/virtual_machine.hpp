#ifndef VIRUTAL_MACHINE_HPP
#define VIRUTAL_MACHINE_HPP

#include <cstdint> // int8_t
#include "bytecode_generator.hpp"

// Data Structures ---------------------------------------------------

struct function_frame{
    function* func;
    int current_scope;

    int8_t* ip; // Pointer to the current instruction
    int end_of_function;
    int current_instruction;

    std::vector<std::map<std::string, double>> variables; // Variables in the current function
};

struct VM{
    double* stack;
    int stack_count;
    int stack_capacity;

    std::vector<function_frame*> function_frames;
};

VM vm; // Statically allocated because only one VM is needed

// -------------------------------------------------------------------

function_frame* get_current_function_frame(){
    return vm.function_frames.back();
}

function_frame* get_function_frame(int index){
    return vm.function_frames[index];
}

function_frame* create_function_frame(function* func){
    function_frame* frame = new function_frame();
    frame->func = func;
    frame->current_scope = 0;
    frame->ip = func->code;
    frame->end_of_function = func->count;
    frame->current_instruction = 0;
    frame->variables.push_back(std::map<std::string, double>());

    return frame;
}

// Initializes the virtual machine ----------------------------------
void init_vm(){
    vm.stack = new double[256];
    vm.stack_count = 0;
    vm.stack_capacity = 256;

    vm.function_frames.clear();
    vm.function_frames.push_back(create_function_frame(function_definitions["main"]));
}

// -------------------------------------------------------------------

//Helper functions ---------------------------------------------------
void vm_error(const std::string& message){
    std::cout << "ERROR: " << message << std::endl;
    exit(1);
}

bool increase_ip(int offset){
    function_frame* frame = get_current_function_frame();
    
    if(frame->ip + offset >= frame->func->code + frame->end_of_function){
        //if in the main function just return
        if(vm.function_frames.size() == 1){
            return false;
        }
        vm_error("End of function reached without return");
    }

    frame->ip += offset;

    return true;
}

//get ip from the current function frame
int8_t* get_ip(){
    return get_current_function_frame()->ip;
}


// Stack operations -------------------------------------------------
void push(double value){
    vm.stack[vm.stack_count++] = value;
}

double pop(){
    return vm.stack[--vm.stack_count];
}

void print_stack(){
    for(int i = 0; i < vm.stack_count; i++){
        std::cout << vm.stack[i] << " ";
    }
    std::cout << std::endl;
}

// -------------------------------------------------------------------

// Map operations -----------------------------------------------------

// Will never go outside of the scope of the function
// Will climb the scope stack from the current scope to the first scope looking for the variable
void set_variable(const std::string& name, double value){
    function_frame* frame = get_current_function_frame();
    frame->variables[frame->current_scope][name] = value;
}

void update_variable(const std::string& name, double value){
    function_frame* frame = get_current_function_frame();
    for(int i = frame->current_scope; i >= 0; i--){
        if(frame->variables[i].find(name) != frame->variables[i].end()){
            frame->variables[i][name] = value;
            return;
        }
    }
    vm_error("Variable not found");
}

double get_variable(const std::string& name){ 
    //std::cout << "Getting variable: " << name << std::endl;
    function_frame* frame = get_current_function_frame();
    //std::cout << "Current scope: " << frame->current_scope << std::endl;
    for(int i = frame->current_scope; i >= 0; i--){
        //std::cout << "Checking scope: " << i << std::endl;
        if(frame->variables[i].find(name) != frame->variables[i].end()){
            return frame->variables[i][name];
        }
    }
    vm_error("Variable not found");
    return 0;
}

// -------------------------------------------------------------------

//--------------------------------------------------------------------

// Runs the virtual machine ------------------------------------------
void run_vm(bool verbose = false){
    if(verbose){
        std::cout << "Running VM" << std::endl;
    }

    while(true){
        if(verbose){
            function_frame* frame = get_current_function_frame();
            int instruction = frame->ip - frame->func->code;

            std::cout << "IP: " << instruction << std::endl;
        }

        switch(*get_ip()){
            // Arithmetic operations
            case OpCode::OP_ADD:
                push(pop() + pop());
                break;
            case OpCode::OP_SUB:
                push(pop() - pop());
                break;
            case OpCode::OP_MUL:
                push(pop() * pop());
                break;
            case OpCode::OP_DIV:
            { // Braces are needed because of the declaration of dividend and divisor
                double dividend = pop();
                double divisor = pop();
                if(divisor == 0){
                    std::cout << "ERROR: Division by zero" << std::endl;
                    return;
                }
                push(dividend / divisor);
                break;
            }

            // Memory operations
            case OpCode::OP_LOAD:
                push(vals.values[get_ip()[1]]);
                increase_ip(1);
                break;
            case OpCode::OP_STORE_VAR:
                // std::cout << "Storing variable: " << variable_names.names[vm.ip[1]] << std::endl;
                // print_stack();
                set_variable(variable_names.names[get_ip()[1]], pop());
                increase_ip(1);
                break;
            case OpCode::OP_UPDATE_VAR:
                update_variable(variable_names.names[get_ip()[1]], pop());
                increase_ip(1);
                break;
            case OpCode::OP_LOAD_VAR:
                push(get_variable(variable_names.names[get_ip()[1]]));
                increase_ip(1);
                break;

            // Control flow operations
            case OpCode::OP_RETURN:
            {
                if(vm.function_frames.size() == 1){
                    //print the return value
                    if(verbose){
                        std::cout << "Exit Code: " << pop() << std::endl;
                    }
                    return;
                }
                
                if(verbose){
                    std::cout << "Returning from function: " << get_current_function_frame()->func->name << std::endl;
                }

                //remove the current function frame
                delete get_current_function_frame();
                vm.function_frames.pop_back();

                //set the ip to the next instruction
                get_current_function_frame()->ip += 1;

                break;
            }
            case OpCode::OP_JUMP:
                increase_ip(get_ip()[1]);
                break;
            case OpCode::OP_JUMP_IF_FALSE:
                if(pop() == 0){
                    increase_ip(get_ip()[1]);
                } else {
                    increase_ip(1);
                }
                break;
            case OpCode::OP_FUNCTION_CALL:
            {
                if(verbose){
                    std::cout << "Calling function: " << get_variable_name(get_ip()[1]) << std::endl;
                }

                
                function* func = function_definitions[get_variable_name(get_ip()[1])];
                vm.function_frames.push_back(create_function_frame(func));

                get_current_function_frame()->ip = func->code - 1; // -1 because the ip will be increased by 1
              
                break;
            }


            // Scope operations
            case OpCode::OP_INC_SCOPE:
                get_current_function_frame()->current_scope++;
                get_current_function_frame()->variables.push_back(std::map<std::string, double>());
                break;
            case OpCode::OP_DEC_SCOPE:
                get_current_function_frame()->current_scope--;
                get_current_function_frame()->variables.pop_back();
                break;

            // Output operations
            case OpCode::OP_PRINT:
                std::cout << pop() << std::endl;
                break;
            
            default:
                std::cout << "ERROR: Unknown opcode" << std::endl;
                return;
        }
        bool ok = increase_ip(1);
        if(!ok){
            break;
        }
    }
}

// -------------------------------------------------------------------

// Starts the interpretation process ---------------------------------
void interpret_bytecode(bool verbose = false){
    init_vm();
    run_vm(verbose);
}

// -------------------------------------------------------------------

#endif // VIRUTAL_MACHINE_HPP