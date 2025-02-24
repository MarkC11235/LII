#ifndef FILES_HPP
#define FILES_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../../helpers/Value.hpp"
#include "../std_lib_helpers/helpers.hpp"

// Global variable declaration
extern std::string directory_path;

// Text files
int file_write(std::string file_path, std::string content);
int file_write_lines(std::string file_path, std::vector<Value> lines);
std::string file_read(std::string file_path);
std::vector<Value> file_read_lines(std::string file_path);

// CSV files
int csv_write(std::string file_path, std::vector<Value> lines);
std::vector<Value> csv_read(std::string file_path);

// JSON files
std::string map_to_json(std::map<std::string, Value> map);
std::map<std::string, Value> json_to_map(std::string json);
Value get_json_value(std::string json, int &i, int json_size);
std::string value_to_json(Value value);
std::vector<Value> json_vector(std::string json, int &i, int json_size);

// Misc files
int run_python_file(std::string file_path);
std::string stdin_read();

#endif // FILES_HPP