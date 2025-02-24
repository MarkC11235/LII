#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>

enum TokenType {
    PRINT_TOKEN,
    LET_TOKEN,
    CONST_TOKEN,
    ASSIGNMENT_TOKEN,
    DEFINE_TOKEN,
    AS_TOKEN,
    FUNC_TOKEN,
    MAP_TOKEN,
    ACCESSOR_TOKEN,
    WHILE_TOKEN,
    FOR_TOKEN,
    FOREACH_TOKEN,
    COLON_TOKEN,
    IN_TOKEN,
    BREAK_TOKEN,
    CONTINUE_TOKEN,
    IF_TOKEN,
    ELSE_IF_TOKEN,
    ELSE_TOKEN,
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
        case TokenType::DEFINE_TOKEN:
            return "DEFINE";
        case TokenType::AS_TOKEN:
            return "AS";
        case TokenType::FUNC_TOKEN:
            return "FUNC";
        case TokenType::MAP_TOKEN:
            return "MAP";
        case TokenType::ACCESSOR_TOKEN:
            return "ACCESSOR";
        case TokenType::WHILE_TOKEN:
            return "WHILE";
        case TokenType::FOR_TOKEN:
            return "FOR";
        case TokenType::FOREACH_TOKEN:
            return "FOREACH";
        case TokenType::COLON_TOKEN:
            return "COLON";
        case TokenType::IN_TOKEN:
            return "IN";
        case TokenType::BREAK_TOKEN:
            return "BREAK";
        case TokenType::CONTINUE_TOKEN:
            return "CONTINUE";
        case TokenType::IF_TOKEN:
            return "IF";
        case TokenType::ELSE_IF_TOKEN:
            return "ELSE_IF";
        case TokenType::ELSE_TOKEN:
            return "ELSE";
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


#endif // TOKEN_HPP