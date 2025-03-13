#ifndef TEST_FRAMEWORK_HPP
#define TEST_FRAMEWORK_HPP

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <sstream>
#include "test_utils.hpp"

template<typename T>
class TestFramework {
public:
    using CompareFunc = std::function<bool(const T&, const T&)>;
    using ReadFunc = std::function<std::vector<T>(const std::string&)>;
    using ProcessFunc = std::function<std::vector<T>(const std::string&)>;
    using ErrorFunc = std::function<void(const std::string&, const std::string&)>;
    using ToStringFunc = std::function<std::string(const T&)>;

    TestFramework(
        const std::string& source_file,
        const std::string& input_extension,
        const std::string& output_extension,
        CompareFunc compare_func,
        ReadFunc read_func,
        ProcessFunc process_func,
        ErrorFunc error_func,
        ToStringFunc to_string_func
    ) : source_file(source_file),
        input_extension(input_extension),
        output_extension(output_extension),
        compare_func(compare_func),
        read_func(read_func),
        process_func(process_func),
        error_func(error_func),
        to_string_func(to_string_func) {}

    void run_tests(const std::string& component) {
        std::vector<std::string> test_files = find_test_files(source_file, input_extension);
        std::vector<std::string> output_files = find_test_files(source_file, output_extension);
        
        if (test_files.empty()) {
            error_func(component, "No test files found in tests directory");
            return;
        }
        
        if (test_files.size() != output_files.size()) {
            error_func(component, "Number of test files and output files do not match");
            return;
        }

        size_t passed_tests = 0;
        size_t total_tests = test_files.size();
        
        for (const auto& test_file : test_files) {
            bool test_passed = true;
            
            auto result = process_func(test_file);
            std::string output_file = test_file.substr(0, test_file.size() - input_extension.size()) + output_extension;
            auto expected = read_func(output_file);

            if (result.size() != expected.size()) {
                error_func(test_file, "Size mismatch - Expected: " + 
                          std::to_string(expected.size()) + " Got: " + 
                          std::to_string(result.size()));
                test_passed = false;
                continue;
            }

            for (size_t i = 0; i < result.size(); i++) {
                if (!compare_func(result[i], expected[i])) {
                    std::stringstream error_msg;
                    error_msg << "Results do not match at position " << i << "\n"
                             << "Expected: " << to_string_func(expected[i]) << "\n"
                             << "Got: " << to_string_func(result[i]);
                    error_func(test_file, error_msg.str());
                    test_passed = false;
                    break;
                }
            }

            if (test_passed) {
                passed_tests++;
            }
        }

        std::cout << "\nTest Summary for " << component << ": " 
                  << passed_tests << "/" << total_tests << " tests passed" 
                  << std::endl;
    }
    
private:
    std::string source_file;
    std::string input_extension;
    std::string output_extension;
    CompareFunc compare_func;
    ReadFunc read_func;
    ProcessFunc process_func;
    ErrorFunc error_func;
    ToStringFunc to_string_func;
};

#endif // TEST_FRAMEWORK_HPP