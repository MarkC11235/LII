#include <string>
#include <vector>
#include <fstream>

#include "cl_exe_file.hpp"

void cl_exe_error(std::string error_message){
    std::cout << "Error: " << error_message << std::endl;
    exit(1);
}

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
            //find all escape sequences and convert them into the actual characters
            std::string new_value = "";
            for(int i = 0; i < value.length(); i++){
                if(value[i] == '\\'){
                    i++;
                    if(value[i] == 'n'){
                        new_value += '\n';
                    }
                    else if(value[i] == 't'){
                        new_value += '\t';
                    }
                    else if(value[i] == '\\'){
                        new_value += '\\';
                    }
                    else if(value[i] == '"'){
                        new_value += '"';
                    }
                    else{
                        cl_exe_error("Invalid escape character");
                        return nullptr;
                    }
                }
                else{
                    new_value += value[i];
                }
            }
            constant = Value(Value_Type::STRING, new_value);
        } else if(type == "function"){ 
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
        std::string type = get_value_type_string(constant);
        std::string value = VALUE_AS_STRING(constant);

        if(type == "string"){ // this is the case, we need to find all escape sequences and covent them into the textual format instead of writing them (Ex: \n instead of an actual newline)
            std::string new_value = "";
            for(int i = 0; i < value.length(); i++){
                if(value[i] == '\n'){
                    new_value += "\\n";
                }
                else if(value[i] == '\t'){
                    new_value += "\\t";
                }
                else if(value[i] == '\\'){
                    new_value += "\\\\";
                }
                else if(value[i] == '"'){
                    new_value += "\\\"";
                }
                else{
                    new_value += value[i];
                }
            }
            file << type << "|" << new_value << std::endl;    
        }
        else{
            file << type << "|" << value << std::endl;
        }

        // file << get_value_type_string(constant) << "|" << VALUE_AS_STRING(constant) << std::endl;
    }

    //write the main bytecode array
    file << main->count << std::endl;
    for(int i = 0; i < main->count; i++){
        file << CODE_TO_NUMBER_STRING(main->code[i]) << std::endl;
    }

    file.close();
}

