#include <chrono>
#include <iostream>
#include <map>
#include <vector>

#include "./VM/virtual_machine.hpp"

int main(int argc, char *argv[]) {

    // Parse arguments
    int args_count = 0;
    std::vector<std::string> args;

    for (int i = 2; i < argc; i++)
    {
        if (std::string(argv[i]).find("-") == 0)
        {
            break;
        }
        args.push_back(argv[i]);
        args_count++;
    }

    // Parse flags
    bool verboseV = false;
    bool debug = false;
    bool time = false;
    bool jit = false;
    int calls_to_jit = 10;
    int stack_capacity = 256;

    for (int i = 2; i < argc; i++)
    {
        if (std::string(argv[i]) == "-v")
        {
            verboseV = true;
        }
        else if (std::string(argv[i]) == "-d")
        {
            debug = true;
        }
        else if (std::string(argv[i]) == "-t")
        {
            time = true;
        }
        else if (std::string(argv[i]) == "-jit")
        {
            jit = true;
        }
        else if (std::string(argv[i]) == "-calls_to_jit")
        {
            calls_to_jit = std::stoi(argv[i + 1]);
            i++;
        }
        else if (std::string(argv[i]) == "-stack_capacity")
        {
            stack_capacity = std::stoi(argv[i + 1]);
            i++;
        }
    }


    // Interpret the bytecode
    std::string input_file = argv[1];
    if (input_file.find(".cl_exe") == std::string::npos) {
        std::cerr << "Input file must be a .cl_exe file" << std::endl;
        exit(1);
    }
    auto start = std::chrono::high_resolution_clock::now();
    interpret_bytecode("./" + input_file, 
                            verboseV, 
                            debug, 
                            jit, 
                            calls_to_jit, 
                            stack_capacity,
                            args_count,
                            args);
    auto end = std::chrono::high_resolution_clock::now();
    if (time) {
        std::cout << "Interpretation took "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " milliseconds." << std::endl;
    }

    return 0;
}