#ifndef CALC_EXE_FILE_HPP
#define CALC_EXE_FILE_HPP

#include <string>
#include <vector>
#include <fstream>

#include "Value.hpp"
#include "Function.hpp"

/*
All information needed to run a program is stored in a .cl_exe file
*/
struct cl_exe{
    std::string name;
    
    std::vector<std::string> variable_names;
    std::vector<Value> constants;

    function* main;
};

void cl_exe_error(std::string error_message);

/*
Parses a .cl_exe file and returns a cl_exe struct
*/
cl_exe* read_cl_exe(std::string path);

/*
Writes the output of the compiler to a .cl_exe file
*/
void write_cl_exe(std::string name, std::string path, function* main, std::vector<std::string> variable_names, std::vector<Value> constants);

#endif //CALC_EXE_FILE_HPP