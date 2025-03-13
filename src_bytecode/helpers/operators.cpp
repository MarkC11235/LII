#include "operators.hpp"

std::map<std::string, std::tuple<int, std::string>> get_operators() {
    static const std::map<std::string, std::tuple<int, std::string>> operators = {
        {"(", {20, "call"}}, // function call
        {"[", {20, "access"}}, // access map or vector
        {"^", {19, "binary"}}, // exponentiation
        {"u-", {11, "unary"}}, // Unary minus 
        {"*", {10, "binary"}},
        {"/", {10, "binary"}},
        {"%", {10, "binary"}},
        {"+", {9, "binary"}},
        {"-", {9, "binary"}},
        {"<", {7, "binary"}},
        {">", {7, "binary"}},
        {"<=", {7, "binary"}},
        {">=", {7, "binary"}},
        {"==", {6, "binary"}},
        {"!=", {6, "binary"}},
        {"!", {5, "unary"}},
        {"&&", {4, "binary"}},
        {"||", {3, "binary"}}
    };
    return operators;
}