#ifndef CL_EXE_GENERATOR_HPP
#define CL_EXE_GENERATOR_HPP

#include <cstdint>
#include <limits>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <unordered_map>
#include <filesystem>

#include "../Compiler.hpp"
#include "../helpers/Function.hpp"
#include "../helpers/cl_exe_file.hpp"
#include "../helpers/Value.hpp"

class CLEXEGenerationPass : public CompilerPass {
public:
CLEXEGenerationPass();
    void run(CompilerContext& context) override;
    void gen_test_file(std::string test_file_name, CompilerContext& context) override;
    CompilerContext& read_test_file(std::string test_file_name) override;
    std::tuple<bool, std::string> compare_out_to_expected(CompilerContext& out, CompilerContext& expected) override;
    ~CLEXEGenerationPass() override;
};


#endif // CL_EXE_GENERATOR_HPP