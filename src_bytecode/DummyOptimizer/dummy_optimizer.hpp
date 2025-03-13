#ifndef DUMMY_OPTIMIZER_HPP
#define DUMMY_OPTIMIZER_HPP

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
#include "../helpers/Value.hpp"

class DummyOptimizerPass : public CompilerPass {
public:
DummyOptimizerPass();
    void run(CompilerContext& context) override;
    void gen_test_file(std::string test_file_name, CompilerContext& context) override;
    CompilerContext& read_test_file(std::string test_file_name) override;
    std::tuple<bool, std::string> compare_out_to_expected(CompilerContext& out, CompilerContext& expected) override;
    ~DummyOptimizerPass() override;
};


#endif // DUMMY_OPTIMIZER_HPP