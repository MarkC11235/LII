#include "./tokenizer.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> // Include the algorithm library to use the remove_if function
#include "../helpers/Token.hpp"
#include "../helpers/test_framework.hpp"


std::vector<Token> read_input(std::string file_path, bool verbose = false, bool include = false); // Forward declaration
void print_tokens(std::vector<Token> tokens);
std::vector<Token> read_tokens_file(std::string file_path);

// Class method implementations
TokenizationPass::TokenizationPass(CompilerContext& ctx) 
    : CompilerPass(ctx) {}

void TokenizationPass::run() {
    if (!context.contains("input_file")) {
        CompilerContext::error("No input file provided");
    }
    std::string inputFile = context.get<std::string>("input_file");
    bool verbose = context.contains("verboseT") ? context.get<bool>("verboseT") : false;
    std::vector<Token> tokens = read_input(inputFile, verbose);
    context.set("tokens", tokens);
}

void TokenizationPass::test() {
    auto error_handler = [](const std::string& file, const std::string& message) {
        std::cerr << "Error in " << file << ": " << message << std::endl;
    };

    TestFramework<Token> framework(
        __FILE__,
        ".cl",
        ".tokens",
        Token::equals,
        read_tokens_file,
        [](const std::string& file) { return read_input(file, false); },
        error_handler,
        [](const Token& token) { return token.to_string(); }
    );
    
    framework.run_tests("TokenizationPass");
}

TokenizationPass::~TokenizationPass() {
    // Cleanup implementation if needed
}

std::string removeWhitespace(std::string str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
    return str;
}

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

/*
Takes in a string which is a line of code and a line number
Returns a vector of tokens
*/
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
                tokens.push_back(Token(TokenType::OPERATOR_TOKEN, "[", line_number));
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
                tokens.push_back(Token(TokenType::OPERATOR_TOKEN, "(", line_number));
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
            case '%':
            case '^':
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
                    //check for escape characters
                    if(input[i] == '\\'){
                        i++;
                        if(i == int(input.length())){
                            return std::vector<Token>{Token(TokenType::ERROR_TOKEN, "No closing quotes", line_number)};
                        }
                        if(input[i] == 'n'){
                            string += '\n';
                        }
                        else if(input[i] == 't'){
                            string += '\t';
                        }
                        else if(input[i] == '\\'){
                            string += '\\';
                        }
                        else if(input[i] == '"'){
                            string += '"';
                        }
                        else{
                            return std::vector<Token>{Token(TokenType::ERROR_TOKEN, "Invalid escape character", line_number)};
                        }
                    }
                    else{
                        string += input[i];
                    }
                    i++;
                }
                if(i == int(input.length())){
                    return std::vector<Token>{Token(TokenType::ERROR_TOKEN, "No closing quotes", line_number)};
                }
                tokens.push_back(Token(TokenType::STRING_TOKEN, string, line_number));
            }
                break;
            case ':':
                tokens.push_back(Token(TokenType::COLON_TOKEN, ":", line_number));
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
                        if(
                            !(i < int(input.length()) 
                            && (isalnum(input[i]) || input[i] == '_')
                            && input[i] != ' ')
                          ){
                            break;
                        }

                        identifier += input[i];
                        i++;
                    }
                    i--;
                    if(identifier == "if"){
                        tokens.push_back(Token(TokenType::IF_TOKEN, "if", line_number));
                    }
                    else if(identifier == "else"){
                        //check if its an else if
                        if(i + 3 < int(input.length()) && input[i+1] == ' ' && input[i + 2] == 'i' && input[i + 3] == 'f'){
                            tokens.push_back(Token(TokenType::ELSE_IF_TOKEN, "else if", line_number));
                            i += 3;
                        }
                        else{
                            tokens.push_back(Token(TokenType::ELSE_TOKEN, "else", line_number));
                        }
                    }
                    else if(identifier == "return"){
                        tokens.push_back(Token(TokenType::RETURN_TOKEN, "return", line_number));
                    }
                    else if(identifier == "define"){
                        tokens.push_back(Token(TokenType::DEFINE_TOKEN, "define", line_number));
                    }
                    else if(identifier == "as"){
                        tokens.push_back(Token(TokenType::AS_TOKEN, "as", line_number));
                    }
                    else if(identifier == "while"){
                        tokens.push_back(Token(TokenType::WHILE_TOKEN, "while", line_number));
                    }
                    else if(identifier == "for"){
                        tokens.push_back(Token(TokenType::FOR_TOKEN, "for", line_number));
                    }
                    else if(identifier == "foreach"){
                        tokens.push_back(Token(TokenType::FOREACH_TOKEN, "foreach", line_number));
                    }
                    else if(identifier == "in"){
                        tokens.push_back(Token(TokenType::IN_TOKEN, "in", line_number));
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
                    else if(identifier == "map"){
                        tokens.push_back(Token(TokenType::MAP_TOKEN, "map", line_number));
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

/*
Takes in the current vector of tokens whenever an include token is found (#"file_name.clh")
Tokenizes the included file and returns a new vector of tokens with the included file's tokens inserted in place of the include token
*/
std::vector<Token> includes(std::vector<Token> tokens, std::string file_path, bool verbose = false){
    std::vector<Token> new_tokens;
    for(int i = 0; i < int(tokens.size()); i++){
        if(tokens[i].get_type() == TokenType::INCLUDE_TOKEN){
            std::string include_file = tokens[i].get_value();
            // std::vector<Token> include_tokens = read_input("./" + include_file, verbose, true);
            //get directory of the file 
            std::string directory = file_path.substr(0, file_path.find_last_of("/\\"));
            std::vector<Token> include_tokens = read_input(directory + "/" + include_file, verbose, true);
            new_tokens.insert(new_tokens.end(), include_tokens.begin(), include_tokens.end());
        }
        else{
            new_tokens.push_back(tokens[i]);
        }
    }
    return new_tokens;
}

/*
Takes in a file path and returns a vector of tokens
*/
std::vector<Token> read_input(std::string file_path, bool verbose, bool include){
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

    File.close();

    // Recursively include files
    tokens = includes(tokens, file_path, verbose);

    // Add an EOF token to the end of the file
    if(include == false){
        tokens.push_back(Token(TokenType::EOF_TOKEN, "EOF", line_number));
    }

    return tokens;
}

std::vector<Token> read_tokens_file(std::string file_path){
    // Open the file
    std::ifstream File(file_path);
    if (!File) {
        std::cout << "Unable to open file " << file_path << std::endl;
        return std::vector<Token>();
    }

    // one token per line
    // number that corresponds to enum, symbol, line number
    std::string current_line;
    std::vector<Token> tokens;
    
    while (std::getline(File, current_line)) {
        std::vector<std::string> token_data;
        std::string current_token = "";
        for (char c : current_line) {
            if (c == ',') {
                token_data.push_back(current_token);
                current_token = "";
            } else {
                current_token += c;
            }
        }
        token_data.push_back(current_token);
        if (token_data.size() != 3) {
            std::cout << "Invalid token data in file " << file_path << std::endl;
            return std::vector<Token>();
        }
        TokenType type = string_to_token_type(token_data[0]);
        tokens.push_back(Token(type, token_data[1], std::stoi(token_data[2])));
    }

    File.close();
    return tokens;
}