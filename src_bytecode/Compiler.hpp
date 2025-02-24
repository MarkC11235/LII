#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <vector>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <any>
#include <string>

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