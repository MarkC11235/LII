#ifndef PARSER_HPP
#define PARSER_HPP

#include "../Compiler.hpp"

class ParsingPass : public CompilerPass {
    public:
        ParsingPass(CompilerContext& ctx);  
        void run() override;            
        void test() override;         
        ~ParsingPass() override;            
    };

#endif // PARSER_HPP