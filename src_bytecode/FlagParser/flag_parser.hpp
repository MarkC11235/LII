#ifndef FLAG_PARSER_HPP
#define FLAG_PARSER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "../Compiler.hpp"

class FlagParsingPass : public CompilerPass
{
private:
    int argc;
    char **argv;

public:
    FlagParsingPass(int argc, char *argv[])
        : CompilerPass(), argc(argc), argv(argv) {}

    void run(CompilerContext &context) override
    {
        if (argc < 2)
        {
            std::cout << "Usage: " << argv[0] << " <input_file.cl> [arg0 arg1 ...] -d -v [-vT -vP -vB -vV] -jit [num] -cs [num]" << std::endl;
            exit(1);
        }

        std::string input_file = argv[1];
        if (input_file.substr(input_file.find_last_of(".") + 1) != "cl")
        {
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
        for (int i = 2; i < argc; i++)
        {
            if (std::string(argv[i]).find("-") == 0)
            {
                arg_start = i;
                break;
            }
            args.push_back(argv[i]);
            args_count++;
        }

        for (int i = arg_start + args_count; i < argc; i++)
        {
            if (std::string(argv[i]) == "-v")
            {
                verboseT = true;
                verboseP = true;
                verboseB = true;
                verboseV = true;
            }
            else if (std::string(argv[i]) == "-vT")
            {
                verboseT = true;
            }
            else if (std::string(argv[i]) == "-vP")
            {
                verboseP = true;
            }
            else if (std::string(argv[i]) == "-vB")
            {
                verboseB = true;
            }
            else if (std::string(argv[i]) == "-vV")
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
                if (i + 1 < argc)
                {
                    try
                    {
                        calls_to_jit = std::stoi(argv[i + 1]);
                    }
                    catch (std::invalid_argument e)
                    {
                        std::cout << "Invalid argument for -jit flag" << std::endl;
                        exit(1);
                    }
                }
            }
            else if (std::string(argv[i]) == "-cs")
            {
                if (i + 1 < argc)
                {
                    try
                    {
                        stack_capacity = std::stoi(argv[i + 1]);
                    }
                    catch (std::invalid_argument e)
                    {
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

    void gen_test_file(std::string test_file_name, CompilerContext &context) override
    {
        std::cout << "FlagParsingPass gen_test_file not implemented" << std::endl;
    }

    CompilerContext &read_test_file(std::string test_file_name) override
    {
        std::cout << "FlagParsingPass read_test_file not implemented" << std::endl;
        CompilerContext *context = new CompilerContext();
        return *context;
    }

    std::tuple<bool, std::string> compare_out_to_expected(CompilerContext &out, CompilerContext &expected) override
    {
        std::cout << "FlagParsingPass compare_out_to_expected not implemented" << std::endl;
        return std::make_tuple(true, "");
    }
};

void Parse_Flags(CompilerContext &context, int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <input_file.cl> [arg0 arg1 ...] -d -v [-vT -vP -vB -vV] -jit [num] -cs [num] -gen_tests -run_tests" << std::endl;
        exit(1);
    }

    std::string input_file = argv[1];
    if (input_file.substr(input_file.find_last_of(".") + 1) != "cl")
    {
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
    bool gen_tests = false;
    bool run_tests = false;

    int arg_start = 2;
    for (int i = 2; i < argc; i++)
    {
        if (std::string(argv[i]).find("-") == 0)
        {
            arg_start = i;
            break;
        }
        args.push_back(argv[i]);
        args_count++;
    }

    for (int i = arg_start + args_count; i < argc; i++)
    {
        if (std::string(argv[i]) == "-v")
        {
            verboseT = true;
            verboseP = true;
            verboseB = true;
            verboseV = true;
        }
        else if (std::string(argv[i]) == "-vT")
        {
            verboseT = true;
        }
        else if (std::string(argv[i]) == "-vP")
        {
            verboseP = true;
        }
        else if (std::string(argv[i]) == "-vB")
        {
            verboseB = true;
        }
        else if (std::string(argv[i]) == "-vV")
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
            if (i + 1 < argc)
            {
                try
                {
                    calls_to_jit = std::stoi(argv[i + 1]);
                }
                catch (std::invalid_argument e)
                {
                    std::cout << "Invalid argument for -jit flag" << std::endl;
                    exit(1);
                }
            }
        }
        else if (std::string(argv[i]) == "-cs")
        {
            if (i + 1 < argc)
            {
                try
                {
                    stack_capacity = std::stoi(argv[i + 1]);
                }
                catch (std::invalid_argument e)
                {
                    std::cout << "Invalid argument for -cs flag" << std::endl;
                    exit(1);
                }
            }
        }
        else if(std::string(argv[i]) == "-gen_tests") {
            gen_tests = true;
        }
        else if(std::string(argv[i]) == "-run_tests") {
            run_tests = true;
        }
    }

    // TODO: Make a single flags map 
    // EX:
    // flags = {
    //     "verboseT": verboseT,
    //     "verboseP": verboseP,
    //     ...} ;
    //
    // context.set("flags", flags);


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
    context.set("gen_tests", gen_tests);
    context.set("run_tests", run_tests);
}

#endif // FLAG_PARSER_HPP