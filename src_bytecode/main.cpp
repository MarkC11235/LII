#include <chrono>
#include <iostream>
#include <map>
#include <vector>

#include "./Compiler.hpp"
#include "./FlagParser/flag_parser.hpp"
#include "./Tokenizer/tokenizer.hpp"
#include "./Parser/parser.hpp"
#include "./BytecodeGenerator/bytecode_generator.hpp"
#include "./VM/virtual_machine.hpp"

int main(int argc, char *argv[]) {
    Compiler compiler;

    compiler.add_pass(new FlagParsingPass(argc, argv));
    compiler.add_pass(new TokenizationPass());
    compiler.add_pass(new ParsingPass());
    compiler.add_pass(new BytecodeGenerationPass());

    compiler.gen_test_files("tester1");
    std::cout << "\n\n" << std::endl;

    // compiler.run();
    compiler.run_tests("tester1");
    // return 0;

    // // Interpret the bytecode
    // auto start = std::chrono::high_resolution_clock::now();
    // std::string input_file = compiler.get_context().get<std::string>("input_file");
    // input_file = input_file.substr(0, input_file.find_last_of(".")) + ".cl_exe";
    // interpret_bytecode("./" + input_file, 
    //                    compiler.get_context().get<bool>("verboseV"), 
    //                    compiler.get_context().get<bool>("debug"),
    //                    compiler.get_context().get<bool>("jit"), 
    //                    compiler.get_context().get<int>("calls_to_jit"),
    //                    compiler.get_context().get<int>("stack_capacity"), 
    //                    compiler.get_context().get<int>("args_count"),
    //                    compiler.get_context().get<std::vector<std::string>>("args"));
    // auto end = std::chrono::high_resolution_clock::now();
    // if (compiler.get_context().get<bool>("verboseV") || compiler.get_context().get<bool>("time")) {
    //     std::cout << "Interpretation took "
    //               << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
    //               << " milliseconds." << std::endl;
    // }

    return 0;
}