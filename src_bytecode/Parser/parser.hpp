#ifndef PARSER_HPP
#define PARSER_HPP

#include "../Compiler.hpp"

class ParsingPass : public CompilerPass {
    public:
        ParsingPass();
        void run(CompilerContext& context) override;
        void gen_test_file(std::string test_file_name, CompilerContext& context) override;
        CompilerContext& read_test_file(std::string test_file_name) override;
        std::tuple<bool, std::string> compare_out_to_expected(CompilerContext& out, CompilerContext& expected) override;        
        ~ParsingPass() override;            
    };

#endif // PARSER_HPP