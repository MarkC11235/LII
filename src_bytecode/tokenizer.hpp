#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> // Include the algorithm library to use the remove_if function

class Token;

enum TokenType {
    PRINT_TOKEN,
    LET_TOKEN,
    CONST_TOKEN,
    ASSIGNMENT_TOKEN,
    FUNC_TOKEN,
    STRUCT_TOKEN,
    ACCESSOR_TOKEN,
    WHILE_TOKEN,
    FOR_TOKEN,
    BREAK_TOKEN,
    CONTINUE_TOKEN,
    IF_TOKEN,
    ELSE_TOKEN,
    EQUALS_TOKEN,
    LESS_THAN_TOKEN,
    GREATER_THAN_TOKEN,
    LESS_THAN_OR_EQUAL_TOKEN,
    GREATER_THAN_OR_EQUAL_TOKEN,
    NOT_EQUAL_TOKEN,
    OPENPAR_TOKEN,
    CLOSEPAR_TOKEN,
    OPENBRACKET_TOKEN,
    CLOSEBRACKET_TOKEN,
    OPENSQUAREBRACKET_TOKEN,
    CLOSESQUAREBRACKET_TOKEN,
    COMMA_TOKEN,
    SEMICOLON_TOKEN,
    RETURN_TOKEN,
    OPERATOR_TOKEN,
    NUMBER_TOKEN,
    BOOL_TOKEN,
    NULL_TOKEN,
    IDENTIFIER_TOKEN,
    STRING_TOKEN,
    FUNCTION_TOKEN,
    STD_LIB_TOKEN,
    INCLUDE_TOKEN,
    ERROR_TOKEN,
    EOF_TOKEN
};

// Helper functions ---------------------------------------------------
std::string token_type_to_string(TokenType type){
    switch(type){
        case TokenType::PRINT_TOKEN:
            return "PRINT";
        case TokenType::LET_TOKEN:
            return "LET";
        case TokenType::CONST_TOKEN:
            return "CONST";
        case TokenType::ASSIGNMENT_TOKEN:
            return "ASSIGNMENT";
        case TokenType::FUNC_TOKEN:
            return "FUNC";
        case TokenType::STRUCT_TOKEN:
            return "STRUCT";
        case TokenType::ACCESSOR_TOKEN:
            return "ACCESSOR";
        case TokenType::WHILE_TOKEN:
            return "WHILE";
        case TokenType::FOR_TOKEN:
            return "FOR";
        case TokenType::BREAK_TOKEN:
            return "BREAK";
        case TokenType::CONTINUE_TOKEN:
            return "CONTINUE";
        case TokenType::IF_TOKEN:
            return "IF";
        case TokenType::ELSE_TOKEN:
            return "ELSE";
        case TokenType::EQUALS_TOKEN:
            return "EQUALS";    
        case TokenType::LESS_THAN_TOKEN:
            return "LESS_THAN";
        case TokenType::GREATER_THAN_TOKEN: 
            return "GREATER_THAN";
        case TokenType::LESS_THAN_OR_EQUAL_TOKEN:
            return "LESS_THAN_OR_EQUAL";
        case TokenType::GREATER_THAN_OR_EQUAL_TOKEN:
            return "GREATER_THAN_OR_EQUAL";
        case TokenType::NOT_EQUAL_TOKEN:
            return "NOT_EQUAL";
        case TokenType::OPENPAR_TOKEN:
            return "OPENPAR";
        case TokenType::CLOSEPAR_TOKEN:
            return "CLOSEPAR";
        case TokenType::OPENBRACKET_TOKEN:
            return "OPENBRACKET";
        case TokenType::CLOSEBRACKET_TOKEN:
            return "CLOSEBRACKET";
        case TokenType::OPENSQUAREBRACKET_TOKEN:
            return "OPENSQUAREBRACKET";
        case TokenType::CLOSESQUAREBRACKET_TOKEN:
            return "CLOSESQUAREBRACKET";
        case TokenType::COMMA_TOKEN:
            return "COMMA";
        case TokenType::SEMICOLON_TOKEN:
            return "SEMICOLON";
        case TokenType::RETURN_TOKEN:
            return "RETURN";
        case TokenType::OPERATOR_TOKEN:
            return "OPERATOR";
        case TokenType::NUMBER_TOKEN:
            return "NUMBER";
        case TokenType::BOOL_TOKEN:
            return "BOOL";
        case TokenType::NULL_TOKEN:
            return "NULL";
        case TokenType::IDENTIFIER_TOKEN:
            return "IDENTIFIER";
        case TokenType::STRING_TOKEN:
            return "STRING";
        case TokenType::FUNCTION_TOKEN:
            return "FUNCTION";
        case TokenType::STD_LIB_TOKEN:
            return "STD_LIB";
        case TokenType::INCLUDE_TOKEN:
            return "INCLUDE";
        case TokenType::ERROR_TOKEN:
            return "ERROR";
        case TokenType::EOF_TOKEN:
            return "EOF";
    }
    return "UNKNOWN";
}

std::string removeWhitespace(std::string str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
    return str;
}

// -------------------------------------------------------------------

// Data structures ---------------------------------------------------
class Token{
    TokenType type;
    std::string value;
    int line_number;
public:
    Token(TokenType type, std::string value, int line_number){
        this->type = type;
        this->value = value;
        this->line_number = line_number;
    }
    TokenType get_type(){
        return this->type;
    }
    std::string get_value(){
        return this->value;
    }
    int get_line_number(){
        return this->line_number;
    }

    void print(){
        std::cout << "Type: " << token_type_to_string(this->type) << " --- Value: " << this->value << " --- Line: " << this->line_number << std::endl;
    }
};

// -------------------------------------------------------------------

// Forward declarations ----------------------------------------------
std::vector<Token> analyze(std::string input, int line_number);
std::vector<Token> includes(std::vector<Token> tokens, bool verbose);
std::vector<Token> read_input(std::string file_path, bool verbose, bool include);

void print_tokens(std::vector<Token> tokens){
    for(Token token : tokens){
        token.print();
    }
    std::cout << std::endl;
}

void tokenization_error(std::string error_message, int line_number, Token token){
    std::cout << "Error on line " << line_number << ": " << error_message << std::endl;
    token.print();
    exit(1);
}

// Tokenizer ---------------------------------------------------------
std::vector<Token> analyze(std::string input, int line_number){
    std::vector<Token> tokens;
    for(int i = 0; i < int(input.length()); i++){
        switch(input[i]){
            case '#':{
                //Next should be a string
                std::string include_file = "";
                i++;
                if(input[i] != '"'){
                    return std::vector<Token>{Token(TokenType::ERROR_TOKEN, "Invalid include statement", line_number)};
                }
                i++;
                while(input[i] != '"' && i < int(input.length())){
                    include_file += input[i];
                    i++;
                }
                if(i == int(input.length())){
                    return std::vector<Token>{Token(TokenType::ERROR_TOKEN, "No closing quotes", line_number)};
                }
                tokens.push_back(Token(TokenType::INCLUDE_TOKEN, include_file, line_number));
            }
                break;  
            case '$':
                tokens.push_back(Token(TokenType::STD_LIB_TOKEN, "$", line_number));
                break;
            case ',':
                tokens.push_back(Token(TokenType::COMMA_TOKEN, ",", line_number));
                break;
            case ' ':
                break;
            case '[':
                tokens.push_back(Token(TokenType::OPENSQUAREBRACKET_TOKEN, "[", line_number));
                break;
            case ']':
                tokens.push_back(Token(TokenType::CLOSESQUAREBRACKET_TOKEN, "]", line_number));
                break;
            case ';':
                tokens.push_back(Token(TokenType::SEMICOLON_TOKEN, ";", line_number));
                break;
            case '=':
                if(i + 1 < int(input.length()) && input[i + 1] == '='){
                    tokens.push_back(Token(TokenType::OPERATOR_TOKEN, "==", line_number));
                    i++;
                }
                else{
                    tokens.push_back(Token(TokenType::ASSIGNMENT_TOKEN, "=", line_number));
                }
                break;
            case '(':
                tokens.push_back(Token(TokenType::OPENPAR_TOKEN, "(", line_number));
                break;
            case ')':
                tokens.push_back(Token(TokenType::CLOSEPAR_TOKEN, ")", line_number));
                break;
            case '{':
                tokens.push_back(Token(TokenType::OPENBRACKET_TOKEN, "{", line_number));
                break;
            case '}':
                tokens.push_back(Token(TokenType::CLOSEBRACKET_TOKEN, "}", line_number));
                break;
            case '+':
            case '-':
            case '*':
                tokens.push_back(Token(TokenType::OPERATOR_TOKEN, std::string(1, input[i]), line_number));
                break;
            case '/':
                if(i + 1 < int(input.length()) && input[i + 1] == '/'){
                    return tokens; // Ignore the rest of the line if a comment is found
                }
                tokens.push_back(Token(TokenType::OPERATOR_TOKEN, std::string(1, input[i]), line_number));
                break;
            case '<':
                if(i + 1 < int(input.length()) && input[i + 1] == '='){
                    tokens.push_back(Token(TokenType::OPERATOR_TOKEN, "<=", line_number));
                    i++;
                }
                else{
                    tokens.push_back(Token(TokenType::OPERATOR_TOKEN, "<", line_number));
                }
                break;
            case '>':
                if(i + 1 < int(input.length()) && input[i + 1] == '='){
                    tokens.push_back(Token(TokenType::OPERATOR_TOKEN, ">=", line_number));
                    i++;
                }
                else{
                    tokens.push_back(Token(TokenType::OPERATOR_TOKEN, ">", line_number));
                }
                break;
            case '!':
                if(i + 1 < int(input.length()) && input[i + 1] == '='){
                    tokens.push_back(Token(TokenType::OPERATOR_TOKEN, "!=", line_number));
                    i++;
                }
                else{
                    tokens.push_back(Token(TokenType::OPERATOR_TOKEN, "!", line_number));
                }
                break;
            case '&':
                if(i + 1 < int(input.length()) && input[i + 1] == '&'){
                    tokens.push_back(Token(TokenType::OPERATOR_TOKEN, "&&", line_number));
                    i++;
                }
                else{
                    return std::vector<Token>{Token(TokenType::ERROR_TOKEN, "error", line_number)};
                }
                break;
            case '|':
                if(i + 1 < int(input.length()) && input[i + 1] == '|'){
                    tokens.push_back(Token(TokenType::OPERATOR_TOKEN, "||", line_number));
                    i++;
                }
                else{
                    return std::vector<Token>{Token(TokenType::ERROR_TOKEN, "error", line_number)};
                }
                break;
            case '"':
            {
                std::string string = "";
                i++;
                while(input[i] != '"' && i < int(input.length())){
                    string += input[i];
                    i++;
                }
                if(i == int(input.length())){
                    return std::vector<Token>{Token(TokenType::ERROR_TOKEN, "No closing quotes", line_number)};
                }
                //std::cout << string << std::endl;
                tokens.push_back(Token(TokenType::STRING_TOKEN, string, line_number));
            }
                break;
            default:
                if(isdigit(input[i]) || input[i] == '.'){
                    std::string number = "";
                    while(i < int(input.length()) && isdigit(input[i])){
                        number += input[i];
                        i++;
                    }
                    if(input[i] == '.'){
                        number += input[i];
                        i++;
                    }
                    while(i < int(input.length()) && isdigit(input[i])){
                        number += input[i];
                        i++;
                    }
                    i--;
                    tokens.push_back(Token(TokenType::NUMBER_TOKEN, number, line_number));
                }
                else if(isalpha(input[i])){
                    std::string identifier = "";
                    while(true)
                    {
                        bool clear_identifier = false; // if you just clear inside the if, then the dot gets attached to the next identifier
                        if(!(i < int(input.length()) 
                            && (isalnum(input[i]) || input[i] == '_')
                            && input[i] != ' ')){
                            
                            if(i < int(input.length()) && input[i] == '.'){
                                tokens.push_back(Token(TokenType::IDENTIFIER_TOKEN, identifier, line_number));
                                clear_identifier = true;
                                tokens.push_back(Token(TokenType::ACCESSOR_TOKEN, ".", line_number));
                            }
                            else{
                                break;
                            }
                        }


                        identifier += input[i];
                        if(clear_identifier){
                            identifier = "";
                        }
                        i++;
                        //std::cout << identifier << std::endl;
                    }
                    i--;
                    if(identifier == "if"){
                        tokens.push_back(Token(TokenType::IF_TOKEN, "if", line_number));
                    }
                    else if(identifier == "else"){
                        tokens.push_back(Token(TokenType::ELSE_TOKEN, "else", line_number));
                    }
                    else if(identifier == "return"){
                        tokens.push_back(Token(TokenType::RETURN_TOKEN, "return", line_number));
                    }
                    else if(identifier == "while"){
                        tokens.push_back(Token(TokenType::WHILE_TOKEN, "while", line_number));
                    }
                    else if(identifier == "for"){
                        tokens.push_back(Token(TokenType::FOR_TOKEN, "for", line_number));
                    }
                    else if(identifier == "let"){
                        tokens.push_back(Token(TokenType::LET_TOKEN, "let", line_number));
                    }
                    else if(identifier == "print"){
                        tokens.push_back(Token(TokenType::PRINT_TOKEN, "print", line_number));
                    }
                    else if(identifier == "func"){
                        tokens.push_back(Token(TokenType::FUNC_TOKEN, "func", line_number));
                    }
                    else if(identifier == "true" || identifier == "false"){
                        tokens.push_back(Token(TokenType::BOOL_TOKEN, identifier, line_number));
                    }
                    else if(identifier == "null"){
                        tokens.push_back(Token(TokenType::NULL_TOKEN, "null", line_number));
                    }
                    else if(identifier == "struct"){
                        tokens.push_back(Token(TokenType::STRUCT_TOKEN, "struct", line_number));
                    }
                    else if(identifier == "const"){
                        tokens.push_back(Token(TokenType::CONST_TOKEN, "const", line_number));
                    }
                    else if(identifier == "break"){
                        tokens.push_back(Token(TokenType::BREAK_TOKEN, "break", line_number));
                    }
                    else if(identifier == "continue"){
                        tokens.push_back(Token(TokenType::CONTINUE_TOKEN, "continue", line_number));
                    }
                    else{
                        tokens.push_back(Token(TokenType::IDENTIFIER_TOKEN, identifier, line_number));
                    }
                }
                else if (input[i] != ' '){
                    // ignore the character
                }
                else{
                    return std::vector<Token>{Token(TokenType::ERROR_TOKEN, "Invalid token", line_number)};
                }
        }
    }
    return tokens;
}

std::vector<Token> includes(std::vector<Token> tokens, bool verbose = false){
    std::vector<Token> new_tokens;
    for(int i = 0; i < int(tokens.size()); i++){
        if(tokens[i].get_type() == TokenType::INCLUDE_TOKEN){
            std::string include_file = tokens[i].get_value();
            std::vector<Token> include_tokens = read_input(directory_path + include_file, verbose, true);
            new_tokens.insert(new_tokens.end(), include_tokens.begin(), include_tokens.end());
        }
        else{
            new_tokens.push_back(tokens[i]);
        }
    }
    return new_tokens;
}

std::vector<Token> read_input(std::string file_path, bool verbose = false, bool include = false){
    // Open the file
    std::ifstream File(file_path); 
    if (!File) {
        std::cout << "Unable to open file " << file_path << std::endl;
        return std::vector<Token>();
    }

    std::string current_line;
    std::vector<Token> tokens;
    int line_number = 0;

    // create a vector of non-empty lines
    while (std::getline(File, current_line)) {
        line_number++;
        std::vector<Token> line_tokens = analyze(current_line, line_number);
        if(line_tokens.size() == 1 && line_tokens[0].get_type() == TokenType::ERROR_TOKEN){
            tokenization_error("Invalid token", line_number, line_tokens[0]);
        }
        tokens.insert(tokens.end(), line_tokens.begin(), line_tokens.end());
    }

    // Close the file
    File.close();

    // Recursively include files
    tokens = includes(tokens, verbose);

    // Add an EOF token to the end of the file
    if(include == false){
        tokens.push_back(Token(TokenType::EOF_TOKEN, "EOF", line_number));
    }

    return tokens;
}

// -------------------------------------------------------------------

#endif // TOKENIZER_HPP