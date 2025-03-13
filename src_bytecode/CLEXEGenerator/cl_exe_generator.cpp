#include "cl_exe_generator.hpp"
    
// CLEXEGenerationPass implementation
CLEXEGenerationPass::CLEXEGenerationPass() 
    : CompilerPass() {}

void CLEXEGenerationPass::run(CompilerContext& context) {
    if (!context.contains("input_file")) {
        CompilerContext::error("input_file not set in context");
    }
    std::string inputFile = context.get<std::string>("input_file");

    if (!context.contains("function")) {
        CompilerContext::error("Function not set in context");
    }
    function* func = context.get<function*>("function");

    if (!context.contains("variable_names")) {
        CompilerContext::error("Variable names not set in context");
    }
    std::vector<std::string> variable_names = context.get<std::vector<std::string>>("variable_names");

    if (!context.contains("constants")) {
        CompilerContext::error("Constants not set in context");
    }
    std::vector<Value> constants = context.get<std::vector<Value>>("constants");

    write_cl_exe(inputFile, "./", func, variable_names, constants);
}

void CLEXEGenerationPass::gen_test_file(std::string test_file_name, CompilerContext& context) {
    std::cout << "CLEXEGenerationPass gen_test_file not implemented" << std::endl;
}

CompilerContext& CLEXEGenerationPass::read_test_file(std::string test_file_name) {
    std::cout << "CLEXEGenerationPass read_test_file not implemented" << std::endl;
    CompilerContext* context = new CompilerContext();
    return *context;
}


std::tuple<bool, std::string> CLEXEGenerationPass::compare_out_to_expected(CompilerContext& out, CompilerContext& expected) {
    std::cout << "CLEXEGenerationPass compare_out_to_expected not implemented" << std::endl;
    return std::make_tuple(false, "CLEXEGenerationPass compare_out_to_expected not implemented");
}


CLEXEGenerationPass::~CLEXEGenerationPass() {
    // Cleanup implementation if needed
}


