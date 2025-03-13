#include "dummy_optimizer.hpp"
    
// CLEXEGenerationPass implementation
DummyOptimizerPass::DummyOptimizerPass() 
    : CompilerPass() {}

void DummyOptimizerPass::run(CompilerContext& context) {
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


    // Dummy optimization
    // Add an exclamtion mark to the end of each constant that is a string
    for (int i = 0; i < (int)constants.size(); i++) {
        if (constants[i].type == Value_Type::STRING) {
            constants[i].data = std::get<std::string>(constants[i].data) + "!";
        }
    }

    context.set("function", func);
    context.set("variable_names", variable_names);
    context.set("constants", constants);

    if (context.contains("verbose") && context.get<bool>("verbose")) {
        std::cout << "DummyOptimizerPass ran successfully" << std::endl;
    }
}

void DummyOptimizerPass::gen_test_file(std::string test_file_name, CompilerContext& context) {
    std::cout << "DummyOptimizerPass gen_test_file not implemented" << std::endl;
}

CompilerContext& DummyOptimizerPass::read_test_file(std::string test_file_name) {
    std::cout << "DummyOptimizerPass read_test_file not implemented" << std::endl;
    CompilerContext* context = new CompilerContext();
    return *context;
}


std::tuple<bool, std::string> DummyOptimizerPass::compare_out_to_expected(CompilerContext& out, CompilerContext& expected) {
    std::cout << "DummyOptimizerPass compare_out_to_expected not implemented" << std::endl;
    return std::make_tuple(false, "DummyOptimizerPass compare_out_to_expected not implemented");
}


DummyOptimizerPass::~DummyOptimizerPass() {
    // Cleanup implementation if needed
}


