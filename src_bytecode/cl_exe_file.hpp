#ifndef CALC_EXE_FILE_HPP
#define CALC_EXE_FILE_HPP

#include <string>
#include <vector>
#include <fstream>

#include "Value.hpp"
#include "Function.hpp"

struct cl_exe{
    std::string name;
    
    std::vector<std::string> variable_names;
    std::vector<Value> constants;

    function* main;
};


cl_exe* read_cl_exe(std::string path);
void write_cl_exe(std::string name, std::string path, function* main, std::vector<std::string> variable_names, std::vector<Value> constants);

cl_exe* read_cl_exe(std::string path){
    cl_exe* exe = new cl_exe;

    //open the file
    std::ifstream file;
    file.open(path);

    //read the file name
    std::getline(file, exe->name);

    //read the variable names vector
    std::string variable_names_size;
    std::getline(file, variable_names_size);
    for(int i = 0; i < std::stoi(variable_names_size); i++){
        std::string name;
        std::getline(file, name);
        exe->variable_names.push_back(name);
    }

    //read the constants vector
    std::string constants_size;
    std::getline(file, constants_size);
    for(int i = 0; i < std::stoi(constants_size); i++){
        std::string type_and_value;
        // read the whole line
        std::getline(file, type_and_value);
        std::string type = type_and_value.substr(0, type_and_value.find("|"));
        std::string value = type_and_value.substr(type_and_value.find("|") + 1);
    
        Value constant;

        if(type == "number"){
            constant = Value(Value_Type::NUMBER, std::stod(value));
        } else if(type == "bool"){
            constant = Value(Value_Type::BOOL, value == "true");
        } else if(type == "string"){
            constant = Value(Value_Type::STRING, value);
        } else if(type == "function"){ // PROBLEM
            value = value.substr(value.find("(") + 1);
            function* func = new function;
            std::vector<std::string> arguments;
            while(value.find(",") != std::string::npos && value.find(")") > value.find(",")){
                arguments.push_back(value.substr(0, value.find(",")));
                value = value.substr(value.find(",") + 2);
            }
            arguments.push_back(value.substr(0, value.find(")")));
            func->arguments = arguments;

            std::vector<CODE_SIZE> code;
            value = value.substr(value.find("{") + 1);
            while(value.find(",") != std::string::npos && value.find("}") > value.find(",")){
                code.push_back(std::stoi(value.substr(0, value.find(","))));
                value = value.substr(value.find(",") + 2);
            }

            func->count = code.size();
            func->capacity = code.size();
            func->code = new CODE_SIZE[func->count];
            for(int i = 0; i < func->count; i++){
                func->code[i] = code[i];
            }

            constant = Value(Value_Type::FUNCTION, func);

        } else if(type == "null"){
            constant = Value(Value_Type::NULL_VALUE, nullptr);
        } 

        exe->constants.push_back(constant);
    }
    

    //read the main bytecode array
    exe->main = new function;
    file >> exe->main->count;
    exe->main->capacity = exe->main->count;
    exe->main->code = new CODE_SIZE[exe->main->count];
    for(int i = 0; i < exe->main->count; i++){
        std::string code;
        file >> code;
        exe->main->code[i] = std::stoi(code);
    }

    file.close();

    return exe;
}

void write_cl_exe(std::string name, std::string path, function* main, std::vector<std::string> variable_names, std::vector<Value> constants){
    //create a text file with the name of the program
    std::ofstream file;
    name = name.substr(0, name.find_last_of("."));
    file.open(path + name + ".cl_exe");

    //write the file name
    file << name << std::endl;

    //write the variable names vector
    file << variable_names.size() << std::endl;
    for(auto name : variable_names){
        file << name << std::endl;
    }

    //write the constants vector
    file << constants.size() << std::endl;
    for(Value constant : constants){
        file << get_value_type_string(constant) << "|" << VALUE_AS_STRING(constant) << std::endl;
    }

    //write the main bytecode array
    file << main->count << std::endl;
    for(int i = 0; i < main->count; i++){
        file << CODE_TO_NUMBER_STRING(main->code[i]) << std::endl;
    }

    file.close();
}



#endif //CALC_EXE_FILE_HPP