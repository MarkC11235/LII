#ifndef FLAG_PARSER_HPP
#define FLAG_PARSER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "../Compiler.hpp"

void Parse_Flags(CompilerContext &context, int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <input_file.cl> -v [-vT -vP -vB ] -gen_tests -run_tests -t" << std::endl;
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
    bool time = false;
    bool jit = false;
    bool gen_tests = false;
    bool run_tests = false;

    for (int i = 2; i < argc; i++)
    {
        if (std::string(argv[i]) == "-v")
        {
            verboseT = true;
            verboseP = true;
            verboseB = true;
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
        else if (std::string(argv[i]) == "-t")
        {
            time = true;
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
    context.set("time", time);
    context.set("jit", jit);
    context.set("gen_tests", gen_tests);
    context.set("run_tests", run_tests);
}

#endif // FLAG_PARSER_HPP