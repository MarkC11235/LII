#include <chrono>
#include <iostream>
#include <map>
#include <vector>

#include "./Compiler.hpp"
#include "./FlagParser/flag_parser.hpp"
#include "./Tokenizer/tokenizer.hpp"
#include "./Parser/parser.hpp"
#include "./BytecodeGenerator/bytecode_generator.hpp"

#include "./DummyOptimizer/dummy_optimizer.hpp"

#include "./CLEXEGenerator/cl_exe_generator.hpp"

int main(int argc, char *argv[]) {
    Compiler compiler;

    // Parse flags
    Parse_Flags(compiler.get_context(), argc, argv);

    // Tokens
    compiler.add_pass(new TokenizationPass());
    
    // AST
    compiler.add_pass(new ParsingPass());
    
    // Bytecode
    compiler.add_pass(new BytecodeGenerationPass());
    
    if (compiler.get_context().get<bool>("O!")){
        compiler.add_pass(new DummyOptimizerPass());
    }

    // CLEXE
    compiler.add_pass(new CLEXEGenerationPass());


    // look at flags to see in what mode to run the compiler
    std::string input_file = compiler.get_context().get<std::string>("input_file");
    if (compiler.get_context().get<bool>("gen_tests")) {
        compiler.gen_test_files(input_file);
    }
    else if (compiler.get_context().get<bool>("run_tests")) {
        compiler.run_tests(input_file);
    }
    else if(compiler.get_context().get<bool>("time")) {
        compiler.run_time();
    }
    else {
        compiler.run();
    }

    return 0;
}