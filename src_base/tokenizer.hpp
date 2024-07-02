#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Include the algorithm library to use the remove_if function
#include <algorithm>

enum TokenType {
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
    FUNCTION_TOKEN,
    DIRECTIVE_TOKEN,
    ERROR_TOKEN,
    EOF_TOKEN
};

std::string token_type_to_string(TokenType type){
    switch(type){
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

std::vector<Token> analyze(std::string input, int line_number){
    std::vector<Token> tokens;
    for(int i = 0; i < int(input.length()); i++){
        switch(input[i]){
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
                    tokens.push_back(Token(TokenType::EQUALS_TOKEN, "==", line_number));
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
                    tokens.push_back(Token(TokenType::LESS_THAN_OR_EQUAL_TOKEN, "<=", line_number));
                    i++;
                }
                else{
                    tokens.push_back(Token(TokenType::LESS_THAN_TOKEN, "<", line_number));
                }
                break;
            case '>':
                if(i + 1 < int(input.length()) && input[i + 1] == '='){
                    tokens.push_back(Token(TokenType::GREATER_THAN_OR_EQUAL_TOKEN, ">=", line_number));
                    i++;
                }
                else{
                    tokens.push_back(Token(TokenType::GREATER_THAN_TOKEN, ">", line_number));
                }
                break;
            case '!':
                if(i + 1 < int(input.length()) && input[i + 1] == '='){
                    tokens.push_back(Token(TokenType::NOT_EQUAL_TOKEN, "!=", line_number));
                    i++;
                }
                else{
                    return std::vector<Token>{Token(TokenType::ERROR_TOKEN, "error", line_number)};
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
                    while(i < int(input.length()) && (isalnum(input[i]) || input[i] == '_')){
                        identifier += input[i];
                        i++;
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
                    else{
                        tokens.push_back(Token(TokenType::IDENTIFIER_TOKEN, identifier, line_number));
                    }
                }
                else if(input[i] != ' '){
                    return std::vector<Token>{Token(TokenType::ERROR_TOKEN, "error", line_number)};
                }
        }
    }
    return tokens;
}

std::string removeWhitespace(std::string str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
    return str;
}

// Reads the input file and returns a vector of strings where each string is a line in the file
std::vector<Token> read_input(std::string file_path, bool verbose = false){
    // Open the file
    std::ifstream File(file_path); 
    if (!File) {
        std::cout << "Unable to open file " << file_path << std::endl;
        return std::vector<Token>();
    }

    std::string token;
    std::vector<Token> tokens;
    int line_number = 0;

    while (std::getline(File, token)) {
        // Remove the newline character from the end of the line
        if(token.find("\n") == token.length() - 1){
            token = token.substr(0, token.length() - 1);
        }
       
        //If there is a comment in the line, remove the comment
        size_t comment_pos = token.find("#");
        if(comment_pos != std::string::npos){
            token = token.substr(0, comment_pos);
        }

        while(token.length() > 0){
            // Split the line by spaces
            size_t pos = token.find(" ");
            std::string token_value;
            if(pos != std::string::npos){
                token_value = token.substr(0, pos);
                token = token.substr(pos + 1);
            } else {
                token_value = token;
                token = "";
            }

            // Remove whitespace from the token
            token_value = removeWhitespace(token_value);

            // Analyze the token
            std::vector<Token> analyzed_tokens = analyze(token_value, line_number);
            if(analyzed_tokens.size() == 1 && analyzed_tokens[0].get_type() == TokenType::ERROR_TOKEN){
                std::cout << "Error: Invalid token " << token_value << " on line " << line_number << std::endl;
                return std::vector<Token>();
            }
            for(Token token : analyzed_tokens){
                tokens.push_back(token);
            }
        }
        line_number++;
    }

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


#endif // TOKENIZER_HPP