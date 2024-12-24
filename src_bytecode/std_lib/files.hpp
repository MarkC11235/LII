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
void file_write(std::string file_path, std::string content){
    std::ofstream File(directory_path + file_path);

    File << content;

    File.close();
}

/*
Reads a file and returns the content as a single string
*/
std::string file_read(std::string file_path){
    std::cout << "Reading file: " << directory_path + file_path << std::endl;
    std::ifstream file(directory_path + file_path);
    std::string content(
        (std::istreambuf_iterator<char>(file)), // This creates an input iterator that reads characters from the input stream file.
                                                // file is assumed to be an object of type std::ifstream or any other input stream.
        (std::istreambuf_iterator<char>())      // This creates a default-constructed std::istreambuf_iterator<char>, which acts as an end-of-stream iterator.

    );
    return content;
}


/*
Runs a python file using the python3 command
*/
void run_python_file(std::string file_path){
    std::string command = "python3 " + file_path;
    system(command.c_str());
}


#endif // FILES_HPP