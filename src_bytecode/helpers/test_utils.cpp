#include "test_utils.hpp"
#include <iostream>
#include <cstdio>

std::vector<std::string> find_test_files(const std::string& source_file, const std::string& extension) {
    // Get the directory where the source file is located
    std::string current_dir = source_file.substr(0, source_file.find_last_of("/\\"));
    std::string test_dir = current_dir + "/tests";

    // Use popen to execute ls command and get all files with the given extension
    std::string cmd = "ls " + test_dir + "/*" + extension + " 2>/dev/null";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error: Could not open pipe to execute command" << std::endl;
        return {};
    }

    // Read the output of ls command
    std::vector<std::string> test_files;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        std::string file = buffer;
        // Remove newline character
        if (!file.empty() && file[file.length()-1] == '\n') {
            file.erase(file.length()-1);
        }
        test_files.push_back(file);
    }
    pclose(pipe);

    return test_files;
}