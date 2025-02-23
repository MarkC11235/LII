#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <vector>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <any>
#include <string>

#include "tokenizer.hpp"
#include "parser.hpp"
#include "bytecode_generator.hpp"
#include "Function.hpp"

class CompilerContext {
public:
    std::unordered_map<std::string, std::any> data;

    template<typename T>
    void set(const std::string& key, const T& value) {
        data[key] = value;
    }

    template<typename T>
    T get(const std::string& key) const {
        return std::any_cast<T>(data.at(key));
    }

    bool contains(const std::string& key) const {
        return data.find(key) != data.end();
    }
};

class CompilerPass {
    protected:
        CompilerContext& context;
    
    public:
        CompilerPass(CompilerContext& ctx) : context(ctx) {}
        virtual ~CompilerPass() {} // Virtual destructor
        virtual void run() = 0;
};

class FlagParsingPass : public CompilerPass {
    private:
        int argc;
        char** argv;
    
    public:
        FlagParsingPass(CompilerContext& ctx, int argc, char* argv[])
            : CompilerPass(ctx), argc(argc), argv(argv) {}
    
        void run() override {
            if (argc < 2) {
                std::cout << "Usage: " << argv[0] << " <input_file.cl> [arg0 arg1 ...] -d -v [-vT -vP -vB -vV] -jit [num] -cs [num]" << std::endl;
                exit(1);
            }
    
            std::string input_file = argv[1];
            if (input_file.substr(input_file.find_last_of(".") + 1) != "cl") {
                std::cout << "Input file must have a .cl extension." << std::endl;
                exit(1);
            }
    
            context.set("input_file", input_file);
    
            bool verboseT = false;
            bool verboseP = false;
            bool verboseB = false;
            bool verboseV = false;
            bool debug = false;
            bool time = false;
            bool jit = false;
            int calls_to_jit = 10;
            int stack_capacity = 256;
            int args_count = 0;
            std::vector<std::string> args;
    
            int arg_start = 2;
            for (int i = 2; i < argc; i++) {
                if (std::string(argv[i]).find("-") == 0) {
                    arg_start = i;
                    break;
                }
                args.push_back(argv[i]);
                args_count++;
            }
    
            for (int i = arg_start + args_count; i < argc; i++) {
                if (std::string(argv[i]) == "-v") {
                    verboseT = true;
                    verboseP = true;
                    verboseB = true;
                    verboseV = true;
                } else if (std::string(argv[i]) == "-vT") {
                    verboseT = true;
                } else if (std::string(argv[i]) == "-vP") {
                    verboseP = true;
                } else if (std::string(argv[i]) == "-vB") {
                    verboseB = true;
                } else if (std::string(argv[i]) == "-vV") {
                    verboseV = true;
                } else if (std::string(argv[i]) == "-d") {
                    debug = true;
                } else if (std::string(argv[i]) == "-t") {
                    time = true;
                } else if (std::string(argv[i]) == "-jit") {
                    jit = true;
                    if (i + 1 < argc) {
                        try {
                            calls_to_jit = std::stoi(argv[i + 1]);
                        } catch (std::invalid_argument e) {
                            std::cout << "Invalid argument for -jit flag" << std::endl;
                            exit(1);
                        }
                    }
                } else if (std::string(argv[i]) == "-cs") {
                    if (i + 1 < argc) {
                        try {
                            stack_capacity = std::stoi(argv[i + 1]);
                        } catch (std::invalid_argument e) {
                            std::cout << "Invalid argument for -cs flag" << std::endl;
                            exit(1);
                        }
                    }
                }
            }
    
            context.set("verboseT", verboseT);
            context.set("verboseP", verboseP);
            context.set("verboseB", verboseB);
            context.set("verboseV", verboseV);
            context.set("debug", debug);
            context.set("time", time);
            context.set("jit", jit);
            context.set("calls_to_jit", calls_to_jit);
            context.set("stack_capacity", stack_capacity);
            context.set("args_count", args_count);
            context.set("args", args);
        }
};

class TokenizationPass : public CompilerPass {
    public:
        TokenizationPass(CompilerContext& ctx)
            : CompilerPass(ctx) {}
    
        void run() override {
            if (!context.contains("input_file")) {
                std::cerr << "Error: input_file not set in context." << std::endl;
                exit(1);
            }
            std::string inputFile = context.get<std::string>("input_file");
            bool verbose = context.contains("verboseT") ? context.get<bool>("verboseT") : false;
            std::vector<Token> tokens = read_input(inputFile, verbose);
            context.set("tokens", tokens);
        }
    
        ~TokenizationPass() override {
            // Clean up if necessary
        }
    };
    
class ParsingPass : public CompilerPass {
    public:
        ParsingPass(CompilerContext& ctx)
            : CompilerPass(ctx) {}
    
        void run() override {
            if (!context.contains("tokens")) {
                std::cerr << "Error: tokens not set in context." << std::endl;
                exit(1);
            }
            bool verbose = context.contains("verboseP") ? context.get<bool>("verboseP") : false;
            std::vector<Token> tokens = context.get<std::vector<Token>>("tokens");
            Node* ast = parse(tokens, verbose);
            context.set("ast", ast);
        }
    
        ~ParsingPass() override {
            if (context.contains("ast")) {
                delete context.get<Node*>("ast");
            }
        }
    };
    
class BytecodeGenerationPass : public CompilerPass {
    public:
        BytecodeGenerationPass(CompilerContext& ctx)
            : CompilerPass(ctx) {}
    
        void run() override {
            if (!context.contains("input_file")) {
                std::cerr << "Error: input_file not set in context." << std::endl;
                exit(1);
            }
            std::string inputFile = context.get<std::string>("input_file");
            if (!context.contains("ast")) {
                std::cerr << "Error: AST not set in context." << std::endl;
                exit(1);
            }
            Node* ast = context.get<Node*>("ast");
            function* func = generate_bytecode(ast, inputFile);
            context.set("function", func);
        }
    
        ~BytecodeGenerationPass() override {
            if (context.contains("function")) {
                delete context.get<function*>("function");
            }
        }
    };

class Compiler {
    private:
        std::vector<CompilerPass*> passes;
        CompilerContext context;
    
    public:
        ~Compiler() {
            for (CompilerPass* pass : passes) {
                delete pass;
            }
        }
    
        void add_pass(CompilerPass* pass) {
            passes.push_back(pass);
        }
    
        void run() {
            for (CompilerPass* pass : passes) {
                pass->run();
            }
        }
    
        void run_time() {
            auto start = std::chrono::high_resolution_clock::now();
            run();
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << "Compilation took "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                      << " milliseconds." << std::endl;
        }
    
        CompilerContext& get_context() {
            return context;
        }
    };

#endif // COMPILER_HPP