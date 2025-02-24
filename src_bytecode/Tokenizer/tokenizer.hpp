#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "../Compiler.hpp"

class TokenizationPass : public CompilerPass {
    public:
        TokenizationPass(CompilerContext& ctx);  
        void run() override;                     
        ~TokenizationPass() override;            
    };

#endif // TOKENIZER_HPP