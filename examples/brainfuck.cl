// initializing the tape
let tape = [];
let TAPE_LENGTH = 1000;

let pointer = 0;
let saved_pointers = [];
let saved_input_positions = [];

for(let i = 0; i < TAPE_LENGTH; i = i + 1){
    tape = $vector_push(tape, 0);
}

print "Created Tape";

let input = "++++. [ > ++++.[-.] <-.]";             
let input_length = $string_len(input);

print "Starting BrainFuck Program | Length = " + input_length;

for(let i = 0; i < input_length; i = i + 1){
    let char = $char_at(input, i);
    
    if(char == "<"){
        if(pointer == 0){
            print "out of bounds left";
            return 1;
        }
        pointer = pointer - 1;
    }

    if(char == ">"){
        if(pointer == TAPE_LENGTH - 1){
            print "out of bounds right";
            return 1;
        }
        pointer = pointer + 1;
    }

    if(char == "+"){
        tape[pointer] = tape[pointer] + 1;
    }

    if(char == "-"){
        tape[pointer] = tape[pointer] - 1;
    }

    if(char == "."){
        print tape[pointer];
    }

    if(char == "["){
        saved_pointers = $vector_push(saved_pointers, pointer);
        saved_input_positions = $vector_push(saved_input_positions, i);
    }

    if(char == "]"){
        if(tape[pointer] != 0){
            pointer = saved_pointers[$vector_len(saved_pointers) - 1];
            i = saved_input_positions[$vector_len(saved_input_positions) - 1];
        }
        else{
            saved_pointers = $vector_pop(saved_pointers);
            saved_input_positions = $vector_pop(saved_input_positions);
        }
    }
}

print "End of BrainFuck Program";