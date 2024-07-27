#ifndef FILES_HPP
#define FILES_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

void file_write(std::string file_path, std::string content){
    std::ofstream File(file_path);

    File << content;

    File.close();
}


// This function doesn't work becasue when the vector is returned for some reason the values inside the vector are missing their data
// I believe this has something to do with the copy constructor, even though it does get called 
// Value file_read(std::string file_path){
//     std::ifstream File(file_path);

//     std::vector<std::string> lines;
//     std::string line;

//     while(std::getline(File, line)){
//         lines.push_back(line);
//     }

//     File.close();

//     Value vals_vector = Value();
//     vals_vector.type = Value_Type::VECTOR;
//     vals_vector.data = std::vector<Value>();

//     for(int i = 0; i < (int)lines.size(); i++){
//         Value v = Value(Value_Type::STRING, lines[i]);
//         std::get<std::vector<Value>>(vals_vector.data).push_back(v);
//     }

//     return vals_vector;
// }

void run_python_file(std::string file_path){
    std::string command = "python3 " + file_path;
    system(command.c_str());
}


#endif // FILES_HPP