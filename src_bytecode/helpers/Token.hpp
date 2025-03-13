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

std::string token_type_to_string(TokenType type);
TokenType string_to_token_type(std::string type);

class Token {
private:
    TokenType type;
    std::string value;
    int line_number;
public:
    Token(TokenType type, std::string value, int line_number);
    TokenType get_type();
    std::string get_value();
    int get_line_number();
    void print();
    std::string to_string() const;
    static bool equals(Token token1, Token token2);
};

#endif // TOKEN_HPP