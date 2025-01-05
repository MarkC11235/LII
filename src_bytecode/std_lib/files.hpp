#ifndef FILES_HPP
#define FILES_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

/*
The directory path of the file that is being executed
This is so the user can use relative paths in the file that is being executed instead of paths relative to the lii executable
*/
std::string directory_path;

// Text files --------------------------------------------------------------------------------------------
/*
Takes in a file path and writes a string to the file
*/
int file_write(std::string file_path, std::string content)
{
    std::ofstream File(directory_path + file_path);

    File << content;

    File.close();

    return 0;
}

/*
Takes in a file path and writes a vector of strings line by line to the file
*/
int file_write_lines(std::string file_path, std::vector<Value> lines)
{
    std::ofstream File(directory_path + file_path);

    for (int i = 0; i < (int)lines.size(); i++)
    {
        File << VALUE_AS_STRING(lines[i]) << std::endl;
    }

    File.close();

    return 0;
}

/*
Reads a file and returns the content as a single string
*/
std::string file_read(std::string file_path)
{
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
std::vector<Value> file_read_lines(std::string file_path)
{
    std::ifstream file(directory_path + file_path);
    std::vector<Value> lines;
    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back({Value_Type::STRING, line});
    }
    return lines;
}
// --------------------------------------------------------------------------------------------------------

// CSV files ---------------------------------------------------------------------------------------------
/*
Writes a vector of vectors of strings to a csv file
*/
int csv_write(std::string file_path, std::vector<Value> lines)
{
    std::ofstream file(directory_path + file_path);

    for (int i = 0; i < (int)lines.size(); i++)
    {
        std::vector<Value> row = VALUE_AS_VECTOR(lines[i]);
        for (int j = 0; j < (int)row.size(); j++)
        {
            file << VALUE_AS_STRING(row[j]);
            if (j != (int)row.size() - 1)
            {
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
std::vector<Value> csv_read(std::string file_path)
{
    std::ifstream file(directory_path + file_path);
    std::vector<Value> lines;
    std::string line;
    while (std::getline(file, line))
    {
        std::vector<Value> row;
        std::string cell;
        for (int i = 0; i < (int)line.size(); i++)
        {
            if (line[i] == ',')
            {
                row.push_back({Value_Type::STRING, cell});
                cell = "";
            }
            else
            {
                cell += line[i];
            }
        }
        row.push_back({Value_Type::STRING, cell});
        lines.push_back({Value_Type::VECTOR, row});
    }
    return lines;
}
// --------------------------------------------------------------------------------------------------------

// JSON files --------------------------------------------------------------------------------------------
std::string map_to_json(std::map<std::string, Value> map);
std::map<std::string, Value> json_to_map(std::string json);
Value get_json_value(std::string json, int &i, int json_size);

/*
Takes in a Value and converts it to a string for json
*/
std::string value_to_json(Value value)
{
    if (get_value_type(value) == Value_Type::MAP)
    {
        return map_to_json(VALUE_AS_MAP(value));
    }
    else if (get_value_type(value) == Value_Type::STRING || get_value_type(value) == Value_Type::FUNCTION)
    {
        return "\"" + VALUE_AS_STRING(value) + "\"";
    }
    else if (get_value_type(value) == Value_Type::VECTOR)
    {
        std::string json = "[";
        std::vector<Value> vec = VALUE_AS_VECTOR(value);
        for (int i = 0; i < (int)vec.size(); i++)
        {
            json += value_to_json(vec[i]);
            if (i != (int)vec.size() - 1)
            {
                json += ",";
            }
        }
        json += "]";
        return json;
    }
    else
    {
        return VALUE_AS_STRING(value);
    }
}

/*
Takes in a map and converts it to a json string
*/
std::string map_to_json(std::map<std::string, Value> map)
{
    std::string json = "{";
    for (auto it = map.begin(); it != map.end(); it++)
    {
        json += "\"" + it->first + "\":";
        json += value_to_json(it->second);
        if (it != --map.end())
        {
            json += ",";
        }
    }
    json += "}";
    return json;
}

/*
Takes in a json string representation of a vector and converts it to a vector of Values
*/
std::vector<Value> json_vector(std::string json, int &i, int json_size)
{
    std::vector<Value> vec;
    while (i < json_size && json[i] != ']')
    {
        // remove whitespace
        while (i < json_size && json[i] == ' ')
        {
            i++;
        }
        Value value = get_json_value(json, i, json_size);
        vec.push_back(value);
        //remove whitespace
        while (i < json_size && json[i] == ' ')
        {
            i++;
        }
        if (json[i] == ',')
        {
            i++;
        }
    }
    return vec;
}

/*
Gets a Value from a json string representation of the value
*/
Value get_json_value(std::string json, int &i, int json_size)
{
    Value value;
    switch (json[i])
    {
    case '{': // map
    {
        std::string sub_json;
        int open_brackets = 1;
        sub_json += json[i];
        i++;
        while (open_brackets > 0)
        {
            if (json[i] == '{')
            {
                open_brackets++;
            }
            else if (json[i] == '}')
            {
                open_brackets--;
            }
            sub_json += json[i];
            i++;
        }
        value = {Value_Type::MAP, json_to_map(sub_json)};
    }
    break;
    case '[': // vector
    {
        i++;
        value = {Value_Type::VECTOR, json_vector(json, i, json_size)};
        if (json[i] != ']')
        {
            std_lib_error("json_to_map", "invalid json format, expected closing bracket");
        }
        i++;
    }
    break;
    case '"': // string (TODO: handle escape characters)
    {
        i++;
        std::string str;
        while (i < json_size && json[i] != '"')
        {
            str += json[i];
            i++;
        }
        i++; // skip closing quote
        value = {Value_Type::STRING, str};
    }
    break;
    case 't': // true
    {
        i++;
        if (i + 3 < json_size && json.substr(i, 3) == "rue")
        {
            value = {Value_Type::BOOL, true};
            i += 3;
        }
        else
        {
            std_lib_error("json_to_map", "invalid json format, expected true");
        }
    }
    break;
    case 'f': // false
    {
        i++;
        if (i + 4 < json_size && json.substr(i, 4) == "alse")
        {
            value = {Value_Type::BOOL, false};
            i += 4;
        }
        else
        {
            std_lib_error("json_to_map", "invalid json format, expected false");
        }
    }
    break;
    case 'n': // null
    {
        i++;
        if (i + 3 < json_size && json.substr(i, 3) == "ull")
        {
            value = {Value_Type::NULL_VALUE, nullptr};
            i += 3;
        }
        else
        {
            std_lib_error("json_to_map", "invalid json format, expected null");
        }
    }
    break;
    default: // number
    {
        std::string num;
        if (json[i] == '-')
        {
            num += json[i];
            i++;
        }
        int num_decimals = 0;
        while (i < json_size && (isdigit(json[i]) || json[i] == '.'))
        {
            if (json[i] == '.')
            {
                num_decimals++;
                if (num_decimals > 1)
                {
                    std_lib_error("json_to_map", "invalid json format, expected number");
                    return value;
                }
            }
            num += json[i];
            i++;
        }
        value = {Value_Type::NUMBER, std::stod(num)};
    }
    break;
    }

    return value;
}

/*
Takes in a json string representation of a map and converts it to a map of strings to Values
*/
std::map<std::string, Value> json_to_map(std::string json)
{
    std::map<std::string, Value> map;

    // check for opening bracket
    if (json[0] != '{')
    {
        std_lib_error("json_to_map", "invalid json format, expected opening bracket");
        return map;
    }

    // parse key value pairs
    int json_size = (int)json.size();
    for (int i = 1; i < json_size - 1; i++)
    {
        std::string key;
        Value value;

        // get key
        // check for closing bracket, allow an empty map and a comma at the end
        if (json[i] == '}')
        {
            break;
        }

        if (json[i] != '"')
        {
            std_lib_error("json_to_map", "invalid json format, expected opening quote");
            return map;
        }
        i++;

        while (json[i] != '"')
        {
            key += json[i];
            i++;
            if (i >= json_size)
            {
                std_lib_error("json_to_map", "invalid json format, expected closing quote");
                return map;
            }
        }
        i++; // skip closing quote

        if (json[i] != ':')
        {
            std_lib_error("json_to_map", "invalid json format, expected colon");
            return map;
        }
        i++; // skip colon

        // get value
        value = get_json_value(json, i, json_size);

        map[key] = value;

        // check for comma
        if (json[i] != ',' && json[i] != '}')
        {
            std_lib_error("json_to_map", "invalid json format, expected comma, or closing bracket, got [" + std::string(1, json[i]) + "]");
            return map;
        }
        i++;

        // skip any whitespace
        while (i < json_size && json[i] == ' ')
        {
            i++;
        }
        i--; // decrement to account for the increment at the end of the loop
    }

    // check for closing bracket
    if (json[json.size() - 1] != '}')
    {
        std_lib_error("json_to_map", "invalid json format");
        return map;
    }

    return map;
}
// --------------------------------------------------------------------------------------------------------

// Misc files --------------------------------------------------------------------------------------------
/*
Runs a python file using the python3 command
*/
int run_python_file(std::string file_path)
{
    std::string command = "python3 " + file_path;
    system(command.c_str());

    return 0;
}

/*
Reads a line from stdin
Waits for the user to press enter
*/
std::string stdin_read()
{
    std::string input;
    std::getline(std::cin, input);
    return input;
}
// --------------------------------------------------------------------------------------------------------

#endif // FILES_HPP