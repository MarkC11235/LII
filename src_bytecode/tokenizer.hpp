#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> // Include the algorithm library to use the remove_if function

enum TokenType {
    PRINT_TOKEN,
    LET_TOKEN,
    ASSIGNMENT_TOKEN,
    WHILE_TOKEN,
    FOR_TOKEN,
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
    IDENTIFIER_TOKEN,
    STRING_TOKEN,
    FUNCTION_TOKEN,
    DIRECTIVE_TOKEN,
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
        case TokenType::ASSIGNMENT_TOKEN:
            return "ASSIGNMENT";
        case TokenType::WHILE_TOKEN:
            return "WHILE";
        case TokenType::FOR_TOKEN:
            return "FOR";
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
        case TokenType::IDENTIFIER_TOKEN:
            return "IDENTIFIER";
        case TokenType::STRING_TOKEN:
            return "STRING";
        case TokenType::FUNCTION_TOKEN:
            return "FUNCTION";
        case TokenType::DIRECTIVE_TOKEN:
            return "DIRECTIVE";
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
            case '#':
                return tokens; // Ignore the rest of the line if a comment is found
            case '$':
                tokens.push_back(Token(TokenType::FUNCTION_TOKEN, "$", line_number));
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
            case '@':
            {
                std::string directive = "";
                while(i < int(input.length()) && input[i] != ' '){
                    directive += input[i];
                    i++;
                }
                i--;
                tokens.push_back(Token(TokenType::DIRECTIVE_TOKEN, directive, line_number));
            }
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
            case '/':
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
                    return std::vector<Token>{Token(TokenType::ERROR_TOKEN, "error", line_number)};
                }
                break;
            case '"':
            {
                std::string string = "";
                i++;
                while(input[i] != '"'){
                    string += input[i];
                    i++;
                }
                //std::cout << string << std::endl;
                tokens.push_back(Token(TokenType::STRING_TOKEN, string, line_number));
            }
                break;
            default:
                if(isdigit(input[i])){
                    std::string number = "";
                    while(i < int(input.length()) && isdigit(input[i])){
                        number += input[i];
                        i++;
                    }
                    i--;
                    tokens.push_back(Token(TokenType::NUMBER_TOKEN, number, line_number));
                }
                else if(isalpha(input[i])){
                    std::string identifier = "";
                    while(  i < int(input.length()) 
                            && (isalnum(input[i]) || input[i] == '_')
                            && input[i] != ' ')
                    {
                        identifier += input[i];
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
                    else{
                        tokens.push_back(Token(TokenType::IDENTIFIER_TOKEN, identifier, line_number));
                    }
                }
        }
    }
    return tokens;
}

std::vector<Token> read_input(std::string file_path, bool verbose = false){
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

    // Add an EOF token to the end of the file
    tokens.push_back(Token(TokenType::EOF_TOKEN, "EOF", line_number));

    // Close the file
    File.close();

    if(verbose){
        std::cout << "\nTokens: " << std::endl;
        for(Token token : tokens){
            token.print();
        }
        std::cout << std::endl;
    }


    return tokens;
}

// -------------------------------------------------------------------

#endif // TOKENIZER_HPP