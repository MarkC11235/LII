#ifndef FILES_HPP
#define FILES_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

// file paths must be relative to the lii executable
// TODO: make the file paths relative to the file that is being executed

std::string directory_path; // the directory path of the file that is being executed
                            // this is so the user can use relative paths in the file that is being executed instead of paths relative to the lii executable

/*
Takes in a file path and writes a string to the file
*/
int file_write(std::string file_path, std::string content){
    std::ofstream File(directory_path + file_path);

    File << content;

    File.close();

    return 0;
}

/*
Takes in a file path and writes a vector of strings line by line to the file
*/
int file_write_lines(std::string file_path, std::vector<Value> lines){
    std::ofstream File(directory_path + file_path);

    for(int i = 0; i < (int)lines.size(); i++){
        File << VALUE_AS_STRING(lines[i]) << std::endl;
    }

    File.close();

    return 0;
}

/*
Reads a file and returns the content as a single string
*/
std::string file_read(std::string file_path){
    //std::cout << "Reading file: " << directory_path + file_path << std::endl;
    std::ifstream file(directory_path + file_path);
    std::string content(
        (std::istreambuf_iterator<char>(file)), // This creates an input iterator that reads characters from the input stream file.
                                                // file is assumed to be an object of type std::ifstream or any other input stream.
        (std::istreambuf_iterator<char>())      // This creates a default-constructed std::istreambuf_iterator<char>, which acts as an end-of-stream iterator.

    );
    return content;
}

/*
Reads a file and returns the lines as a vector of strings
*/
std::vector<Value> file_read_lines(std::string file_path){
    std::ifstream file(directory_path + file_path);
    std::vector<Value> lines;
    std::string line;
    while(std::getline(file, line)){
        lines.push_back({Value_Type::STRING, line});
    }
    return lines;
}

/*
Reads a line from stdin
*/
std::string stdin_read(){
    std::string input;
    std::getline(std::cin, input);
    return input;
}

/*
Writes a vector of vectors of strings to a csv file
*/
int csv_write(std::string file_path, std::vector<Value> lines){
    std::ofstream file(directory_path + file_path);

    for(int i = 0; i < (int)lines.size(); i++){
        std::vector<Value> row = VALUE_AS_VECTOR(lines[i]);
        for(int j = 0; j < (int)row.size(); j++){
            file << VALUE_AS_STRING(row[j]);
            if(j != (int)row.size() - 1){
                file << ",";
            }
        }
        file << std::endl;
    }

    file.close();

    return 0;
}

/*
Reads a csv file and returns the content as a vector of vectors of strings
*/
std::vector<Value> csv_read(std::string file_path){
    std::ifstream file(directory_path + file_path);
    std::vector<Value> lines;
    std::string line;
    while(std::getline(file, line)){
        std::vector<Value> row;
        std::string cell;
        for(int i = 0; i < (int)line.size(); i++){
            if(line[i] == ','){
                row.push_back({Value_Type::STRING, cell});
                cell = "";
            }else{
                cell += line[i];
            }
        }
        row.push_back({Value_Type::STRING, cell});
        lines.push_back({Value_Type::VECTOR, row});
    }
    return lines;
}

/*
Runs a python file using the python3 command
*/
int run_python_file(std::string file_path){
    std::string command = "python3 " + file_path;
    system(command.c_str());

    return 0;
}



#endif // FILES_HPP