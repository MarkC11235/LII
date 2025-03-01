#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "../Compiler.hpp"

class TokenizationPass : public CompilerPass {
    public:
        TokenizationPass();  
        void run(CompilerContext& context) override;
        void gen_test_file(std::string test_file_name, CompilerContext& context) override;
        CompilerContext& read_test_file(std::string test_file_name) override;
        std::tuple<bool, std::string> compare_out_to_expected(CompilerContext& out, CompilerContext& expected) override;                 
        ~TokenizationPass() override;            
    };

#endif // TOKENIZER_HPP