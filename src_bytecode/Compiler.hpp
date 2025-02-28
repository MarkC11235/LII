#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <vector>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <any>
#include <string>
#include <map>
#include <tuple>

typedef std::unordered_map<std::string, std::any> CompilerData;

class CompilerContext {
public:
    CompilerData data;

    CompilerContext(std::string file_name = "") {
        data["file_name"] = file_name;
    }

    ~CompilerContext() {
        data.clear();
    }

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

    static void error(const std::string& message) {
        std::cerr << "CompilerContext Error: " << message << std::endl;
        exit(1);
    }
};


class CompilerPass {    
    public:
        CompilerPass() {}
        virtual ~CompilerPass() {} // Virtual destructor
        virtual void run(CompilerContext& data) = 0;
        virtual void gen_test_file(std::string test_file_name, CompilerContext& data) = 0;
        virtual CompilerContext& read_test_file(std::string test_file_name) = 0;
        virtual std::tuple<bool, std::string> compare_out_to_expected(CompilerContext& out, CompilerContext& expected) = 0;
        static void error(const std::string& message) {
            std::cerr << "CompilerPass Error: " << message << std::endl;
            exit(1);
        }
};

class Compiler {
    private:
        std::vector<CompilerPass*> passes;
        CompilerContext context;
    
    public:
        Compiler(std::string file_name = "") {
            context.set("file_name", file_name);
        }

        ~Compiler() {
            for (CompilerPass* pass : passes) {
                delete pass;
            }
        }
    
        void add_pass(CompilerPass* pass) {
            passes.push_back(pass);
        }
    
        void run(std::string file_name = "") {
            for (CompilerPass* pass : passes) {
                pass->run(get_context());
            }
        }

        void gen_test_files(std::string test_file_name) {
            for (CompilerPass* pass : passes) {
                pass->run(get_context());
                pass->gen_test_file(test_file_name, get_context());
            }
        }

        void run_tests(std::string test_file_name) {
            for (CompilerPass* pass : passes) {
                pass->run(get_context());
                CompilerContext& expected = pass->read_test_file(test_file_name);
                std::tuple<bool, std::string> result = pass->compare_out_to_expected(get_context(), expected);
                delete &expected;
                if (!std::get<0>(result)) {
                    std::cout << "Test failed: " << std::get<1>(result) << std::endl;
                }
            }
        }
    
        void run_time(std::string file_name = "") {
            auto start = std::chrono::high_resolution_clock::now();
            run(file_name);
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << "Compilation took "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                      << " milliseconds." << std::endl;
        }
    
        CompilerContext& get_context() {
            return context;
        }

        static void error(const std::string& message) {
            std::cerr << "Compiler Error: " << message << std::endl;
            exit(1);
        }
    };

#endif // COMPILER_HPP