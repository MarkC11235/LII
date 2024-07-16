#ifndef VIRUTAL_MACHINE_HPP
#define VIRUTAL_MACHINE_HPP

#include <cstdint> // int8_t

#include "bytecode_generator.hpp"
#include "./std_lib/std_lib.hpp"

// Data Structures ---------------------------------------------------
struct function_frame{
    function* func;
    int current_scope;

    CODE_SIZE* ip; // Pointer to the current instruction
    int end_of_function;
    int current_instruction;

    std::vector<std::map<std::string, Value>> variables; // Variables in the current function
};

struct VM{
    Value* stack;
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
    frame->variables.push_back(std::map<std::string, Value>());

    return frame;
}

// Initializes the virtual machine ----------------------------------
void init_vm(int stack_capacity = 256){
    vm.stack = new Value[stack_capacity]; 
    vm.stack_count = 0;
    vm.stack_capacity = stack_capacity;

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
    
    if(frame->ip + offset >= frame->func->code + frame->end_of_function){ // Probably slow  
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
CODE_SIZE* get_ip(){
    return get_current_function_frame()->ip;
}


// Stack operations -------------------------------------------------
void push(Value value){
    vm.stack[vm.stack_count++] = value;
}

Value pop(){
    return vm.stack[--vm.stack_count];
}

void print_stack(){
    for(int i = 0; i < vm.stack_count; i++){
        print_value(vm.stack[i]);
        std::cout << " ";
    }
    std::cout << std::endl;
}

// -------------------------------------------------------------------

// Map operations -----------------------------------------------------

// Creates a new variable in the current scope
void set_variable(const std::string& name, Value value){
    function_frame* frame = get_current_function_frame();
    frame->variables[frame->current_scope][name] = value;
}

// Updates a variable in the current function frame, does not look in the parent frames
// Looks for the variable in the closest scope, so climbs out of ifs, loops, etc.
void update_variable(const std::string& name, Value value){
    function_frame* frame = get_current_function_frame();
    for(int i = frame->current_scope; i >= 0; i--){
        if(frame->variables[i].find(name) != frame->variables[i].end()){
            frame->variables[i][name] = value;
            return;
        }
    }
    vm_error("Variable not found");
}

// Gets a variable in the current function frame, does not look in the parent frames
// Looks for the variable in the closest scope, so climbs out of ifs, loops, etc.
Value get_variable(const std::string& name){ 
    function_frame* frame = get_current_function_frame();
    for(int i = frame->current_scope; i >= 0; i--){
        if(frame->variables[i].find(name) != frame->variables[i].end()){
            return frame->variables[i][name];
        }
    }
    vm_error("Variable not found");
    return Value(); // To avoid warning, but this line will never be reached because of vm_error
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
            {
                Value a = pop();
                Value b = pop();
                if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){
                    push({Value_Type::NUMBER, std::get<double>(a.data) + std::get<double>(b.data)});
                }
                else if(a.type == Value_Type::STRING || b.type == Value_Type::STRING){
                    push({Value_Type::STRING, VALUE_AS_STRING(a) + VALUE_AS_STRING(b)});
                } 
                else {
                    vm_error("Invalid types for addition");
                }
                break;
            }
            case OpCode::OP_SUB:
            {
                Value a = pop();
                Value b = pop();
                if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){
                    push({Value_Type::NUMBER, std::get<double>(a.data) - std::get<double>(b.data)});
                } else {
                    vm_error("Invalid types for subtraction");
                }
                break;
            }
            case OpCode::OP_MUL:
            {
                Value a = pop();
                Value b = pop();
                if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){
                    push({Value_Type::NUMBER, std::get<double>(a.data) * std::get<double>(b.data)});
                } else {
                    vm_error("Invalid types for multiplication");
                }
                break;
            }
            case OpCode::OP_DIV:
            {
                Value a = pop();
                Value b = pop();
                if(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER){
                    if(std::get<double>(b.data) == 0){
                        vm_error("Division by zero");
                    }
                    push({Value_Type::NUMBER, std::get<double>(a.data) / std::get<double>(b.data)});
                } else {
                    vm_error("Invalid types for division");
                }
                break;
            }

            // Comparison operations
            // Can only compare numbers
            case OpCode::OP_EQ:
            {
                Value a = pop();
                Value b = pop();
                if(!(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)){
                    vm_error("Invalid types for comparison");
                }
                    
                push({Value_Type::NUMBER, (double)(VALUE_AS_NUMBER(a) == VALUE_AS_NUMBER(b))});
                break;
            }
            case OpCode::OP_NEQ:
            {
                Value a = pop();
                Value b = pop();
                if(!(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)){
                    vm_error("Invalid types for comparison");
                }

                push({Value_Type::NUMBER, (double)(VALUE_AS_NUMBER(a) != VALUE_AS_NUMBER(b))});
                break;
            }
            case OpCode::OP_GT:
            {
                Value a = pop();
                Value b = pop();
                if(!(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)){
                    vm_error("Invalid types for comparison");
                }
                
                push({Value_Type::NUMBER, (double)(VALUE_AS_NUMBER(a) > VALUE_AS_NUMBER(b))});
                break;
            }
            case OpCode::OP_LT:
            {
                Value a = pop();
                Value b = pop();
                if(!(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)){
                    vm_error("Invalid types for comparison");
                }
                
                push({Value_Type::NUMBER, (double)(VALUE_AS_NUMBER(a) < VALUE_AS_NUMBER(b))});
                break;
            }
            case OpCode::OP_GTEQ:
            {
                Value a = pop();
                Value b = pop();
                if(!(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)){
                    vm_error("Invalid types for comparison");
                }
                
                push({Value_Type::NUMBER, (double)(VALUE_AS_NUMBER(a) >= VALUE_AS_NUMBER(b))});
                break;
            }
            case OpCode::OP_LTEQ:
            {
                Value a = pop();
                Value b = pop();
                if(!(a.type == Value_Type::NUMBER && b.type == Value_Type::NUMBER)){
                    vm_error("Invalid types for comparison");
                }
                
                push({Value_Type::NUMBER, (double)(VALUE_AS_NUMBER(a) <= VALUE_AS_NUMBER(b))});
                break;
            }


            // Memory operations
            case OpCode::OP_LOAD:
                //push(vals.values[get_ip()[1]]);
                push(get_constant(get_ip()[1]));
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
                        std::cout << "Exit Code: ";
                        print_value(pop());
                        std::cout << std::endl;
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
            {
                bool val = VALUE_AS_BOOL(pop());
                if(!val){
                    increase_ip(get_ip()[1]);
                } else {
                    increase_ip(1);
                }
                break;
            }
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
            
            case OpCode::OP_STD_LIB_CALL:
            {
                if(verbose){
                    std::cout << "Calling std lib function: " << STD_LIB_FUNCTION_NAMES[get_ip()[1]] << std::endl;
                }

                switch(get_ip()[1]){
                    //tests ------------------------------------------------
                    case 0: // test
                        push({Value_Type::NUMBER, test()});
                        break;
                    case 1: // inc
                        push({Value_Type::NUMBER, inc(VALUE_AS_NUMBER(pop()))});
                        break;
                    // -----------------------------------------------------

                    // string functions -----------------------------------
                    case 2: // str_concat
                    {
                        Value a = pop();
                        Value b = pop();
                        if(a.type == Value_Type::STRING && b.type == Value_Type::STRING){
                            push({Value_Type::STRING, str_concat(VALUE_AS_STRING(b), VALUE_AS_STRING(a))}); // reverse order because of the stack
                        } else {
                            vm_error("Invalid types for string concatenation");
                        }
                        break;
                    }
                    case 3: // str_substr
                    {
                        Value length = pop();
                        Value start = pop();
                        Value str = pop();
                        if(str.type == Value_Type::STRING && start.type == Value_Type::NUMBER && length.type == Value_Type::NUMBER){
                            push({Value_Type::STRING, str_substr(VALUE_AS_STRING(str), (int)VALUE_AS_NUMBER(start), (int)VALUE_AS_NUMBER(length))});
                        } else {
                            vm_error("Invalid types for string slicing");
                        }
                        break;
                    }
                    case 4: // str_len
                    {
                        Value str = pop();
                        if(str.type == Value_Type::STRING){
                            push({Value_Type::NUMBER, (double)str_len(VALUE_AS_STRING(str))});
                        } else {
                            vm_error("Invalid types for string length");
                        }
                        break;
                    }
                    case 5: // char_at
                    {
                        Value index = pop();
                        Value str = pop();
                        if(str.type == Value_Type::STRING && index.type == Value_Type::NUMBER){
                            push({Value_Type::STRING, char_at(VALUE_AS_STRING(str), (int)VALUE_AS_NUMBER(index))});
                        } else {
                            vm_error("Invalid types for character access");
                        }
                        break;
                    }
                    case 6: // replace_char
                    {
                        Value c = pop();
                        Value index = pop();
                        Value str = pop();
                        if(str.type == Value_Type::STRING && index.type == Value_Type::NUMBER && c.type == Value_Type::STRING){
                            push({Value_Type::STRING, replace_char(VALUE_AS_STRING(str), (int)VALUE_AS_NUMBER(index), VALUE_AS_STRING(c))});
                        } else {
                            vm_error("Invalid types for character replacement");
                        }
                        break;
                    }
                    // -----------------------------------------------------

                    // custom functions -----------------------------------
                    case 7: // print_colored_text
                    {
                        Value color = pop();
                        Value text = pop();
                        if(text.type == Value_Type::STRING && color.type == Value_Type::STRING){
                            print_colored_text(VALUE_AS_STRING(text), VALUE_AS_STRING(color));
                        } else {
                            vm_error("Invalid types for colored text printing");
                        }
                        break;
                    }
                    // -----------------------------------------------------
                    
                    default:
                        vm_error("Unknown std lib function");
                }

                increase_ip(1);

                break;
            }


            // Scope operations
            case OpCode::OP_INC_SCOPE:
                get_current_function_frame()->current_scope++;
                get_current_function_frame()->variables.push_back(std::map<std::string, Value>());
                break;
            case OpCode::OP_DEC_SCOPE:
                get_current_function_frame()->current_scope--;
                get_current_function_frame()->variables.pop_back();
                break;

            // Output operations
            case OpCode::OP_PRINT:
                //pop();
                print_value(pop());
                std::cout << std::endl;
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