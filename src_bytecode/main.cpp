#include <chrono>
#include <iostream>
#include <map>
#include <vector>

#include "Function.hpp"
#include "Value.hpp"
#include "./std_lib/std_lib.hpp" // Include the standard library, first so that the objects are declared before they are used
#include "tokenizer.hpp"
#include "parser.hpp"
#include "bytecode_generator.hpp"
#include "virtual_machine.hpp"
#include "cl_exe_file.hpp"

// TODO: MAKE NULL BE ABLE TO BE COMPARABLE (==, !=)
// TODO: ADD 
//           exit expr ; // exit the program completely and prints the value of expr
// TODO: ADD bitwise operators (&, |, ^, ~, <<, >>)

int main(int argc, char *argv[]) {
    // Check if the user has provided the input file and verbosity flag
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " <input_file.cl> -d -v [-vT -vP -vB -vV]" << std::endl;
        return 1;
    }
    std::string input_file = argv[1];
    // Check if the input file has the correct extension
    if(input_file.substr(input_file.find_last_of(".") + 1) != "cl") {
        std::cout << "Input file must have a .cl extension." << std::endl;
        return 1;
    }

    bool verboseT = false;
    bool verboseP = false;
    bool verboseB = false;
    bool verboseV = false;

    bool debug = false;

    bool time = false;

    // Check for flags
    for(int i = 2; i < argc; i++) {
        if(std::string(argv[i]) == "-v") {
            verboseT = true;
            verboseP = true;
            verboseB = true;
            verboseV = true;
        } else if(std::string(argv[i]) == "-vT") {
            verboseT = true;
        } else if(std::string(argv[i]) == "-vP") {
            verboseP = true;
        } else if(std::string(argv[i]) == "-vB") {
            verboseB = true;
        } else if(std::string(argv[i]) == "-vV") {
            verboseV = true;
        } else if(std::string(argv[i]) == "-d"){
            debug = true;
        } else if(std::string(argv[i]) == "-t"){
            time = true;
        }
    }

    // set the directory path for the files.hpp functions and tokenize the input when there are include statements
    input_file = "./" + input_file;
    int last_slash = input_file.find_last_of("/");
    directory_path = input_file.substr(0, last_slash + 1);

    // Read the input file and tokenize the input
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Token> tokens = read_input(input_file, verboseT);

    auto end = std::chrono::high_resolution_clock::now();
    if (verboseT || time) {
        std::cout << "\nTokens: " << std::endl;
        for(Token token : tokens){
            token.print();
        }
        std::cout << std::endl;
        std::cout << "Tokenization took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " milliseconds.\n" << std::endl;
    }

    // Parse the tokens and form the AST
    start = std::chrono::high_resolution_clock::now();
    Node* ast = parse(tokens, verboseP);
    end = std::chrono::high_resolution_clock::now();
    if (verboseP || time) {
        std::cout << "Parsing took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " milliseconds.\n" << std::endl;
    }

    // Traverse the AST and generate the bytecode
    start = std::chrono::high_resolution_clock::now();
    function* func = generate_bytecode(ast, input_file);
    end = std::chrono::high_resolution_clock::now();
    if (verboseB || time) {
        std::cout << "Bytecode generation took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " milliseconds.\n" << std::endl;

        std::cout << "Constants:" << std::endl;
        display_constants();
        std::cout << std::endl;

        std::cout << "Variables:" << std::endl;
        display_variables();
        std::cout << std::endl;

        std::cout << "Bytecode:" << std::endl;
        std::cout << "Main Function: " << std::endl;
        display_bytecode(func);
        std::cout << std::endl;
    }

    // Free the memory
    delete ast;

    // Interpret the bytecode
    start = std::chrono::high_resolution_clock::now();
    input_file = input_file.substr(0, input_file.find_last_of(".")) + ".cl_exe";
    interpret_bytecode("./" + input_file, verboseV, debug);
    end = std::chrono::high_resolution_clock::now();
    if (verboseV || time) {
        std::cout << "Interpretation took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " milliseconds." << std::endl;
    }

    return 0;
}
