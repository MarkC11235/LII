#include <chrono>
#include <iostream>
#include <map>
#include <vector>

#include "tokenizer.hpp"
#include "parser.hpp"
#include "bytecode_generator.hpp"
#include "virtual_machine.hpp"

int main(int argc, char *argv[]) {
    // Check if the user has provided the input file and verbosity flag
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " <input_file.calc> [-v]" << std::endl;
        return 1;
    }
    std::string input_file = argv[1];
    bool verbose = false;

    // Check for flags
    for(int i = 2; i < argc; i++) {
        if(std::string(argv[i]) == "-v") {
            verbose = true;
        } 
    }

    // Read the input file and tokenize the input
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Token> tokens = read_input(input_file, verbose);
    auto end = std::chrono::high_resolution_clock::now();
    if (verbose) {
        std::cout << "Tokenization took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " milliseconds." << std::endl << std::endl;
    }

    // Parse the tokens and form the AST
    start = std::chrono::high_resolution_clock::now();
    Node* ast = parse(tokens, verbose);
    end = std::chrono::high_resolution_clock::now();
    if (verbose) {
        std::cout << "Parsing took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " milliseconds." << std::endl << std::endl;
    }

    // Traverse the AST and generate the bytecode
    start = std::chrono::high_resolution_clock::now();
    generate_bytecode(ast);
    end = std::chrono::high_resolution_clock::now();
    if (verbose) {
        std::cout << "Bytecode generation took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " milliseconds." << std::endl;

        std::cout << "Constants:" << std::endl;
        display_constants();
        std::cout << std::endl;

        std::cout << "Variables:" << std::endl;
        display_variables();
        std::cout << std::endl;

        std::cout << "Bytecode:" << std::endl;
        for(auto it = function_definitions.begin(); it != function_definitions.end(); it++){
            std::cout << "Function: " << it->first << std::endl;
            display_bytecode(it->second);
        }
        std::cout << std::endl;
    }


    // Interpret the bytecode
    start = std::chrono::high_resolution_clock::now();
    interpret_bytecode(verbose);
    end = std::chrono::high_resolution_clock::now();
    if (verbose) {
        std::cout << "Interpretation took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " milliseconds." << std::endl;
    }

    // Free the memory
    delete ast;

    return 0;
}
