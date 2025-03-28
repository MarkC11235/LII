#include "Token.hpp"

std::string token_type_to_string(TokenType type) {
    switch(type) {
        case TokenType::PRINT_TOKEN:
            return "PRINT";
        case TokenType::LET_TOKEN:
            return "LET";
        case TokenType::CONST_TOKEN:
            return "CONST";
        case TokenType::GLOBAL_TOKEN:
            return "GLOBAL";
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

TokenType string_to_token_type(std::string type) {
    if(type == "PRINT") {
        return TokenType::PRINT_TOKEN;
    } else if(type == "LET") {
        return TokenType::LET_TOKEN;
    } else if(type == "CONST") {
        return TokenType::CONST_TOKEN;
    } else if(type == "GLOBAL") {
        return TokenType::GLOBAL_TOKEN;
    } else if(type == "ASSIGNMENT") {
        return TokenType::ASSIGNMENT_TOKEN;
    } else if(type == "DEFINE") {
        return TokenType::DEFINE_TOKEN;
    } else if(type == "AS") {
        return TokenType::AS_TOKEN;
    } else if(type == "FUNC") {
        return TokenType::FUNC_TOKEN;
    } else if(type == "MAP") {
        return TokenType::MAP_TOKEN;
    } else if(type == "ACCESSOR") {
        return TokenType::ACCESSOR_TOKEN;
    } else if(type == "WHILE") {
        return TokenType::WHILE_TOKEN;
    } else if(type == "FOR") {
        return TokenType::FOR_TOKEN;
    } else if(type == "FOREACH") {
        return TokenType::FOREACH_TOKEN;
    } else if(type == "COLON") {
        return TokenType::COLON_TOKEN;
    } else if(type == "IN") {
        return TokenType::IN_TOKEN;
    } else if(type == "BREAK") {
        return TokenType::BREAK_TOKEN;
    } else if(type == "CONTINUE") {
        return TokenType::CONTINUE_TOKEN;
    } else if(type == "IF") {
        return TokenType::IF_TOKEN;
    } else if(type == "ELSE_IF") {
        return TokenType::ELSE_IF_TOKEN;
    } else if(type == "ELSE") {
        return TokenType::ELSE_TOKEN;
    } else if(type == "OPENPAR") {
        return TokenType::OPENPAR_TOKEN;
    } else if(type == "CLOSEPAR") {
        return TokenType::CLOSEPAR_TOKEN;
    } else if(type == "OPENBRACKET") {
        return TokenType::OPENBRACKET_TOKEN;
    } else if(type == "CLOSEBRACKET") {
        return TokenType::CLOSEBRACKET_TOKEN;
    } else if(type == "OPENSQUAREBRACKET") {
        return TokenType::OPENSQUAREBRACKET_TOKEN;
    } else if(type == "CLOSESQUAREBRACKET") {
        return TokenType::CLOSESQUAREBRACKET_TOKEN;
    } else if(type == "COMMA") {
        return TokenType::COMMA_TOKEN;
    } else if(type == "SEMICOLON") {
        return TokenType::SEMICOLON_TOKEN;
    } else if(type == "RETURN") {
        return TokenType::RETURN_TOKEN;
    } else if(type == "OPERATOR") {
        return TokenType::OPERATOR_TOKEN;
    } else if(type == "NUMBER") {
        return TokenType::NUMBER_TOKEN;
    } else if(type == "BOOL") {
        return TokenType::BOOL_TOKEN;
    } else if(type == "NULL") {
        return TokenType::NULL_TOKEN;
    } else if(type == "IDENTIFIER") {
        return TokenType::IDENTIFIER_TOKEN;
    } else if(type == "STRING") {
        return TokenType::STRING_TOKEN;
    } else if(type == "FUNCTION") {
        return TokenType::FUNCTION_TOKEN;
    } else if(type == "STD_LIB") {
        return TokenType::STD_LIB_TOKEN;
    } else if(type == "INCLUDE") {
        return TokenType::INCLUDE_TOKEN;
    } else if(type == "ERROR") {
        return TokenType::ERROR_TOKEN;
    } else if(type == "EOF") {
        return TokenType::EOF_TOKEN;
    }
    return TokenType::ERROR_TOKEN;
}

Token::Token(TokenType type, std::string value, int line_number) {
    this->type = type;
    this->value = value;
    this->line_number = line_number;
}

TokenType Token::get_type() {
    return this->type;
}

std::string Token::get_value() {
    return this->value;
}

int Token::get_line_number() {
    return this->line_number;
}

void Token::print() {
    std::cout << "Type: " << token_type_to_string(this->type) 
              << " --- Value: " << this->value 
              << " --- Line: " << this->line_number << std::endl;
}

std::string Token::to_string() const {
    return token_type_to_string(this->type) + "," + this->value + "," + std::to_string(this->line_number);
}

bool Token::equals(Token token1, Token token2) {
    return token1.get_type() == token2.get_type() && token1.get_value() == token2.get_value();
}