#include <chrono>
#include <iostream>
#include <map>
#include <vector>
#include "tokenizer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

int main(int argc, char *argv[]) {
    // Check if the user has provided the input file and verbosity flag
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " <input_file.calc> [-v] [-sif] [-i 'file1.calc.functions' 'file2.calc.functions' ...]" << std::endl;
        return 1;
    }
    std::string input_file = argv[1];
    bool verbose = false;
    bool sif = false;
    std::vector<std::string> function_files;

    // for(int i = 0; i < argc; i++) {
    //     std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    // }

    // Check for flags
    for(int i = 2; i < argc; i++) {
        if(std::string(argv[i]) == "-v") {
            verbose = true;
        } else if(std::string(argv[i]) == "-sif") {
            sif = true;
        } else if(std::string(argv[i]) == "-i") {
            for(int j = i + 1; j < argc; j++) {
                function_files.push_back(argv[j]);
            }
            break;
        }
    }

    // for(const std::string& function_file : function_files) {
    //     std::cout << "Function file: " << function_file << std::endl;
    // }

    // Map to store the function definitions
    std::map<std::string, Node*> functions;

    // Load the function files
    for (const std::string& function_file : function_files) {
        if(verbose){
            std::cout << "Loading function file: " << function_file << std::endl;
        }
        std::vector<Token> function_tokens = read_input(function_file, verbose);
        std::vector<Node*> function_nodes = parse_functions(function_tokens, verbose);
        for (Node* function_node : function_nodes) {
            functions[function_node->get_value()] = function_node; // value 0 is the function name
            if(verbose){
                std::cout << "Function " << function_node->get_value() << " loaded." << std::endl;
            }
        }
    }

    // Read the input file and tokenize the input
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Token> tokens = read_input(input_file, verbose);
    auto end = std::chrono::high_resolution_clock::now();
    if (verbose) {
        std::cout << "Tokenization took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " milliseconds." << std::endl;
    }

    // Parse the tokens and form the AST
    start = std::chrono::high_resolution_clock::now();
    Node* ast = parse(tokens, verbose);
    end = std::chrono::high_resolution_clock::now();
    if (verbose) {
        std::cout << "Parsing took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " milliseconds." << std::endl;
    }

    // Interpret the AST
    start = std::chrono::high_resolution_clock::now();
    std::map<std::string, int> variables;
    interpret(ast, variables, functions, sif);
    end = std::chrono::high_resolution_clock::now();
    if (verbose) {
        std::cout << "Interpretation took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " milliseconds." << std::endl;
    }

    return 0;
}