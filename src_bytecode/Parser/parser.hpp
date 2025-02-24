#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>

#include "../Compiler.hpp"
#include "../std_lib/std_lib.hpp"
#include "../helpers/Token.hpp"
#include "../helpers/Node.hpp"

Node* parse(std::vector<Token> tokens, bool verbose);

class ParsingPass : public CompilerPass {
    public:
        ParsingPass(CompilerContext& ctx)
            : CompilerPass(ctx) {}
    
        void run() override {
            if (!context.contains("tokens")) {
                std::cerr << "Error: tokens not set in context." << std::endl;
                exit(1);
            }
            bool verbose = context.contains("verboseP") ? context.get<bool>("verboseP") : false;
            std::vector<Token> tokens = context.get<std::vector<Token>>("tokens");
            Node* ast = parse(tokens, verbose);
            context.set("ast", ast);
        }
    
        ~ParsingPass() override {
            if (context.contains("ast")) {
                delete context.get<Node*>("ast");
            }
        }
    };

// Helper functions ---------------------------------------------------
int parsing_error(std::string message, Token token);

/*
Returns the first token in the vector
*/
Token peek(std::vector<Token> tokens){
    if(tokens.size() > 0){
        return tokens[0];
    } else {
        return Token(TokenType::EOF_TOKEN, "EOF", -1);
    }
}

/*
Removes the first token in the vector and returns it
*/
Token pop(std::vector<Token>& tokens){
    if(tokens.size() > 0){
        Token token = tokens[0];
        tokens.erase(tokens.begin());
        return token;
    } else {
        return Token(TokenType::EOF_TOKEN, "EOF", -1);
    }
}

/*
Put a token at the beginning of the vector
*/
void place_token_back(std::vector<Token>& tokens, Token token){
    tokens.insert(tokens.begin(), token);
}

/*
maps operators to their precedence and type
*/
std::map<std::string, std::tuple<int, std::string>> operators = {
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
    {"||", {3, "binary"}},
};

/*
The following four functions just look up the operator in the operators map
*/
int precedence(std::string op, Token token){ 
    if(operators.find(op) == operators.end()){
        parsing_error("UNKNOWN operator", token);
    }

    return std::get<0>(operators[op]);
}

bool is_binary_operator(std::string op){
    if(operators.find(op) == operators.end()){
        std::cout << "Unknown Operator: " + op << std::endl;
        exit(1);
    }

    return std::get<1>(operators[op]) == "binary";
}

bool is_unary_operator(std::string op){
    if(operators.find(op) == operators.end()){
        std::cout << "Unknown Operator: " + op << std::endl;
        exit(1);
    }

    return std::get<1>(operators[op]) == "unary";
}

bool is_access_operator(std::string op){
    if(operators.find(op) == operators.end()){
        std::cout << "Unknown Operator: " + op << std::endl;
        exit(1);
    }

    return std::get<1>(operators[op]) == "access";
}

bool is_call_operator(std::string op){
    if(operators.find(op) == operators.end()){
        std::cout << "Unknown Operator: " + op << std::endl;
        exit(1);
    }

    return std::get<1>(operators[op]) == "call";
}

std::vector<std::string> splitStringByComma(const std::string& str) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, ',')) {
        result.push_back(token);
    }

    return result;
}
// -------------------------------------------------------------------



Node* ROOT_NODE;

// Parsing functions -------------------------------------------------

int parsing_error(std::string message, Token token){
    std::cout << message << std::endl;
    std::cout << "Token: " << token.get_value() << " at line " << token.get_line_number() << std::endl;
    ROOT_NODE->print();
    exit(1);
}

void parse_stmt_list(std::vector<Token>& tokens, Node* current);
void parse_stmt(std::vector<Token>& tokens, Node* current);
void parse_expr(std::vector<Token>& tokens, Node* current, bool nested); 
void parse_function_call(std::vector<Token>& tokens, Node* current);
void parse_std_lib_call(std::vector<Token>& tokens, Node* current);
void parse_function(std::vector<Token>& tokens, Node* current);
void parse_assignment(std::vector<Token>& tokens, Node* current, bool is_const = false);
void parse_if(std::vector<Token>& tokens, Node* current);
void parse_return(std::vector<Token>& tokens, Node* current);
void parse_list(std::vector<Token>& tokens, Node* current);
void parse_map(std::vector<Token>& tokens, Node* current);
void parse_accessor(std::vector<Token>& tokens, Node* current);
void parse_variable_update(std::vector<Token>& tokens, Node* current);
void parse_print(std::vector<Token>& tokens, Node* current);

// EXPR PARSER ------------------------------------------------------------------------------------------------------------
    // EXPR GRAMMAR
/*

expression             → logical_or_expression
logical_or_expression  → logical_and_expression ( '||' logical_and_expression )*
logical_and_expression → comparison_expression ( '&&' comparison_expression )*
comparison_expression  → additive_expression ( ( '==' | '!=' | '<' | '>' | '<=' | '>=' ) additive_expression )*
additive_expression    → multiplicative_expression ( ( '+' | '-' ) multiplicative_expression )*
multiplicative_expression → prefix_expression ( ( '*' | '/' | '%' ) prefix_expression )*  
prefix_expression      → ( '!' | '-' ) prefix_expression | exponentiation_expression
exponentiation_expression → postfix_expression ( '^' exponentiation_expression )*  
postfix_expression     → primary ( '[' expression ']' | '(' arguments? ')' )*
primary                → identifier | literal | '(' expression ')'
literal                → NUMBER | STRING | 'null' | 'true' | 'false' | map | function | vector
arguments              → expression ( ',' expression )*

*/


void parse_expr(std::vector<Token>& tokens, Node* current, bool nested);
Node* parse_logical_or_expr(std::vector<Token>& tokens, Node* current);
Node* parse_logical_and_expr(std::vector<Token>& tokens, Node* current);
Node* parse_comparison_expr(std::vector<Token>& tokens, Node* current);
Node* parse_additive_expr(std::vector<Token>& tokens, Node* current);
Node* parse_multiplicative_expr(std::vector<Token>& tokens, Node* current);
Node* parse_prefix_expr(std::vector<Token>& tokens, Node* current);
Node* parse_exponentiation_expr(std::vector<Token>& tokens, Node* current);
Node* parse_postfix_expr(std::vector<Token>& tokens, Node* current);
Node* parse_primary(std::vector<Token>& tokens, Node* current);
Node* parse_literal(std::vector<Token>& tokens, Node* current);
Node* parse_arguments(std::vector<Token>& tokens, Node* current);

std::map<std::string, std::string> op_types = {
    {"||", {"logical_or"}},
    {"&&", {"logical_and"}},
    {"==", {"comparison"}},
    {"!=", {"comparison"}},
    {"<", {"comparison"}},
    {">", {"comparison"}},
    {"<=", {"comparison"}},
    {">=", {"comparison"}},
    {"+", {"additive"}},
    {"-", {"additive"}},
    {"*", {"multiplicative"}},
    {"/", {"multiplicative"}},
    {"%", {"multiplicative"}},
    {"^", {"exponentiation"}},
    {"u!", {"prefix"}},
    {"u-", {"prefix"}},
    {"[", {"postfix"}},
    {"(", {"postfix"}}
};

bool is_operator(std::string op){
    return op_types.find(op) != op_types.end();
}

bool is_op_type(std::string op, std::string type){
    if(op_types.find(op) == op_types.end()){
        // parsing_error("Unknown operator: " + op + "; expected: " + type, Token(TokenType::EOF_TOKEN, "EOF", -1));
        return false;
    }

    return op_types[op] == type;
}

void parse_expr(std::vector<Token>& tokens, Node* current, bool nested = false){
    Node* logical_or_expr = parse_logical_or_expr(tokens, current);
    if(logical_or_expr == nullptr){
        parsing_error("Syntax error: expected expression", Token(TokenType::EOF_TOKEN, "EOF", -1));
    }
    current->add_child(logical_or_expr);
}

Node* parse_logical_or_expr(std::vector<Token>& tokens, Node* current){
    Node* logical_and_expr = parse_logical_and_expr(tokens, current);
    if(logical_and_expr == nullptr){
        return nullptr;
    }

    while(is_op_type(peek(tokens).get_value(), "logical_or") && peek(tokens).get_type() == TokenType::OPERATOR_TOKEN){
        Token token = pop(tokens);
        Node* op = new Node(NodeType::OP_NODE, token.get_value(), token.get_line_number());
        op->add_child(logical_and_expr);
        op->add_child(parse_logical_and_expr(tokens, current));
        logical_and_expr = op;
    }

    return logical_and_expr;
}

Node* parse_logical_and_expr(std::vector<Token>& tokens, Node* current){
    Node* comparison_expr = parse_comparison_expr(tokens, current);
    if(comparison_expr == nullptr){
        return nullptr;
    }

    while(is_op_type(peek(tokens).get_value(), "logical_and") && peek(tokens).get_type() == TokenType::OPERATOR_TOKEN){
        Token token = pop(tokens);
        Node* op = new Node(NodeType::OP_NODE, token.get_value(), token.get_line_number());
        op->add_child(comparison_expr);
        op->add_child(parse_comparison_expr(tokens, current));
        comparison_expr = op;
    }

    return comparison_expr;
}

Node* parse_comparison_expr(std::vector<Token>& tokens, Node* current){
    Node* additive_expr = parse_additive_expr(tokens, current);
    if(additive_expr == nullptr){
        return nullptr;
    }

    while(is_op_type(peek(tokens).get_value(), "comparison") && peek(tokens).get_type() == TokenType::OPERATOR_TOKEN){
        Token token = pop(tokens);
        Node* op = new Node(NodeType::OP_NODE, token.get_value(), token.get_line_number());
        op->add_child(additive_expr);
        op->add_child(parse_additive_expr(tokens, current));
        additive_expr = op;
    }

    return additive_expr;
}

Node* parse_additive_expr(std::vector<Token>& tokens, Node* current){
    Node* multiplicative_expr = parse_multiplicative_expr(tokens, current);
    if(multiplicative_expr == nullptr){
        return nullptr;
    }

    while(is_op_type(peek(tokens).get_value(), "additive") && peek(tokens).get_type() == TokenType::OPERATOR_TOKEN){
        Token token = pop(tokens);
        Node* op = new Node(NodeType::OP_NODE, token.get_value(), token.get_line_number());
        op->add_child(multiplicative_expr);
        op->add_child(parse_multiplicative_expr(tokens, current));
        multiplicative_expr = op;
    }

    return multiplicative_expr;
}

Node* parse_multiplicative_expr(std::vector<Token>& tokens, Node* current){
    Node* prefix_expr = parse_prefix_expr(tokens, current);
    if(prefix_expr == nullptr){
        return nullptr;
    }

    while(is_op_type(peek(tokens).get_value(), "multiplicative") && peek(tokens).get_type() == TokenType::OPERATOR_TOKEN){
        Token token = pop(tokens);
        Node* op = new Node(NodeType::OP_NODE, token.get_value(), token.get_line_number());
        op->add_child(prefix_expr);
        op->add_child(parse_prefix_expr(tokens, current));
        prefix_expr = op;
    }

    return prefix_expr;
}

Node* parse_prefix_expr(std::vector<Token>& tokens, Node* current){
    if(is_op_type("u"+peek(tokens).get_value(), "prefix") && peek(tokens).get_type() == TokenType::OPERATOR_TOKEN){ // kinda janky but it works, cause now in map, not is 'u!'
        Token token = pop(tokens);
        std::string value = token.get_value();
        if(value == "-"){
            value = "u-";
        }
        Node* op = new Node(NodeType::OP_NODE, value, token.get_line_number());
        op->add_child(parse_prefix_expr(tokens, current));
        return op;
    }

    return parse_exponentiation_expr(tokens, current);
}

Node* parse_exponentiation_expr(std::vector<Token>& tokens, Node* current){
    Node* postfix_expr = parse_postfix_expr(tokens, current);
    if(postfix_expr == nullptr){
        return nullptr;
    }

    while(is_op_type(peek(tokens).get_value(), "exponentiation") && peek(tokens).get_type() == TokenType::OPERATOR_TOKEN){
        Token token = pop(tokens);
        Node* op = new Node(NodeType::OP_NODE, token.get_value(), token.get_line_number());
        op->add_child(postfix_expr);
        op->add_child(parse_postfix_expr(tokens, current));
        postfix_expr = op;
    }

    return postfix_expr;
}

Node* parse_postfix_expr(std::vector<Token>& tokens, Node* current){
    Node* primary = parse_primary(tokens, current);
    if(primary == nullptr){
        return nullptr;
    }

    while(is_op_type(peek(tokens).get_value(), "postfix") && peek(tokens).get_type() == TokenType::OPERATOR_TOKEN){
        Token token = pop(tokens);
        Node* op = new Node(NodeType::OP_NODE, token.get_value(), token.get_line_number());
        op->add_child(primary);
        if(op->get_value() == "["){
            Node* expr = new Node(NodeType::EXPR_NODE, "", token.get_line_number());
            parse_expr(tokens, expr);
            op->add_child(expr);

            // check for closing bracket
            Token token = pop(tokens);
            if(token.get_type() != TokenType::CLOSESQUAREBRACKET_TOKEN){
                parsing_error("Syntax error: expected ']'", token);
            }
        }
        else if(op->get_value() == "("){
            Node* list = new Node(NodeType::LIST_NODE, "", token.get_line_number());
            op->add_child(list);
            Token token = peek(tokens);
            if(token.get_type() != TokenType::CLOSEPAR_TOKEN){ // Check if there are parameters
                for(;;){ // Can have 0 or more parameters
                    Node* expr = new Node(NodeType::EXPR_NODE, "", token.get_line_number());
                    parse_expr(tokens, expr);
                    list->add_child(expr);

                    token = peek(tokens);
                    if(token.get_type() == TokenType::CLOSEPAR_TOKEN){ // End of parameters
                        break;
                    } else if(token.get_type() == TokenType::COMMA_TOKEN){
                        pop(tokens);
                    } else {
                        parsing_error("Syntax error: expected ',' or ')'", token);
                    }
                }
            }

            token = pop(tokens);
            if(token.get_type() != TokenType::CLOSEPAR_TOKEN){
                parsing_error("Syntax error: expected ')'", token);
            }
        }

        primary = op;
    }

    return primary;
}

Node* parse_primary(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    if(token.get_value() == "("){ // Nested expression
        Node* expr = new Node(NodeType::EXPR_NODE, "", token.get_line_number());
        parse_expr(tokens, expr);
        token = pop(tokens);
        if(token.get_type() != TokenType::CLOSEPAR_TOKEN){
            parsing_error("Syntax error: expected ')'", token);
        }
        return expr;
    }
    else if(token.get_type() == TokenType::IDENTIFIER_TOKEN){
        Node* var = new Node(NodeType::VAR_NODE, token.get_value(), token.get_line_number());
        return var;
    }
    else {
        place_token_back(tokens, token);
        return parse_literal(tokens, current);
    }
}

Node* parse_literal(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    if(token.get_type() == TokenType::NUMBER_TOKEN){
        return new Node(NodeType::NUM_NODE, token.get_value(), token.get_line_number());
    }
    else if(token.get_type() == TokenType::STRING_TOKEN){
        return new Node(NodeType::STRING_NODE, token.get_value(), token.get_line_number());
    }
    else if(token.get_type() == TokenType::BOOL_TOKEN){
        return new Node(NodeType::BOOL_NODE, token.get_value(), token.get_line_number());;
    }
    else if(token.get_type() == TokenType::NULL_TOKEN){
        return new Node(NodeType::NULL_NODE, "null", token.get_line_number());
    }
    else if(token.get_type() == TokenType::MAP_TOKEN){
        Node* map = new Node(NodeType::MAP_NODE, "", token.get_line_number());
        parse_map(tokens, map);
        return map;
    }
    else if(token.get_type() == TokenType::FUNC_TOKEN){
        Node* function = new Node(NodeType::FUNCTION_NODE, "", token.get_line_number());
        parse_function(tokens, function);
        return function;
    }
    else if(token.get_type() == TokenType::STD_LIB_TOKEN){
        Node* std_lib = new Node(NodeType::STD_LIB_CALL_NODE, token.get_value(), token.get_line_number());
        parse_std_lib_call(tokens, std_lib);
        return std_lib;
    }
    else if(token.get_value() == "["){ // Vector
        Node* list = new Node(NodeType::LIST_NODE, "", token.get_line_number());
        parse_list(tokens, list);
        return list;
    }
    else {
        parsing_error("Syntax error: expected literal", token);
        return nullptr; // to avoid warning, but the program will exit on previous line
    }
}

// -----------------------------------------------------------------------------------------------

/*
Finds type of value and calls the appropriate function to parse it
Types: expression, list, map, function, null
*/
void parse_value(std::vector<Token>& tokens, Node* current){
    Node* expr = new Node(NodeType::EXPR_NODE, "", peek(tokens).get_line_number());
    current->add_child(expr);
    parse_expr(tokens, expr);
}

/*
Parses a function call; Ex: let result = factorial(5);
Allows function to have 0 or more parameters that can be of any type
*/
void parse_function_call(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    if(token.get_type() != TokenType::IDENTIFIER_TOKEN){
        parsing_error("Syntax error: expected identifier", token);
    }

    current->add_value(token.get_value());

    token = pop(tokens);
    if(token.get_value() != "("){
        parsing_error("Syntax error: expected '('", token);
    }

    // Parse the parameters
    Node* list = new Node(NodeType::LIST_NODE, "", token.get_line_number());
    current->add_child(list);
    token = peek(tokens);
    if(token.get_type() != TokenType::CLOSEPAR_TOKEN){ // Check if there are parameters
        for(;;){ // Can have 0 or more parameters
            parse_value(tokens, list);

            token = peek(tokens);
            if(token.get_type() == TokenType::CLOSEPAR_TOKEN){ // End of parameters
                break;
            } else if(token.get_type() == TokenType::COMMA_TOKEN){
                pop(tokens);
            } else {
                parsing_error("Syntax error: expected ',' or ')'", token);
            }
        }
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEPAR_TOKEN){
        parsing_error("Syntax error: expected ')'", token);
    }

}

/*
Same as parse_function_call, but it is prefixed with '$' to indicate that it is a std lib call
*/
void parse_std_lib_call(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    if(token.get_type() != TokenType::IDENTIFIER_TOKEN){
        parsing_error("Syntax error: expected identifier", token);
    }

    current->add_value(token.get_value());

    token = pop(tokens);
    if(token.get_value() != "("){
        parsing_error("Syntax error: expected '('", token);
    }

    // Parse the parameters
    Node* list = new Node(NodeType::LIST_NODE, "", token.get_line_number());
    current->add_child(list);
    token = peek(tokens);
    if(token.get_type() != TokenType::CLOSEPAR_TOKEN){ // Check if there are parameters
        for(;;){ // Can have 0 or more parameters
            parse_value(tokens, list);

            token = peek(tokens);
            if(token.get_type() == TokenType::CLOSEPAR_TOKEN){ // End of parameters
                break;
            } else if(token.get_type() == TokenType::COMMA_TOKEN){
                pop(tokens);
            } else {
                parsing_error("Syntax error: expected ',' or ')'", token);
            }
        }
    }

    // Check if the std_lib call has the correct number of parameters TODO: FIND A BETTER WAY TO DO THIS, DON'T LIKE IT BEING IN THE PARSER
    std::string std_lib_name = current->get_value(1);
    int num_params = list->get_children().size();
    if(is_correct_number_of_parameters(std_lib_name, num_params) == false){
        parsing_error("Syntax error: invalid number of parameters for std lib call: " + std_lib_name, token);
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEPAR_TOKEN){
        parsing_error("Syntax error: expected ')'", token);
    }
}

/*
Parses a function definition; Ex: let f = func factorial(n) { stmt_list };
*/
void parse_function(std::vector<Token>& tokens, Node* function){
    // check for opening parenthesis
    Token token = pop(tokens);

    // '(' is classifed as an operator, so we need to check if it is the correct symbol
    if(token.get_value() != "("){
        parsing_error("Syntax error: expected '('", token);
    }

    // Parse the parameters
    Node* list = new Node(NodeType::LIST_NODE, "", token.get_line_number());
    function->add_child(list);
    token = peek(tokens);
    if(token.get_type() != TokenType::CLOSEPAR_TOKEN){ // Check if there are parameters
        for(;;){ // Can have 0 or more parameters
            token = pop(tokens);
            if(token.get_type() == TokenType::IDENTIFIER_TOKEN){ // Identifier
                Node* var = new Node(NodeType::VAR_NODE, token.get_value(), token.get_line_number());
                list->add_child(var);
            } else {
                parsing_error("Syntax error: expected identifier", token);
            }

            token = peek(tokens);
            if(token.get_type() == TokenType::CLOSEPAR_TOKEN){ // End of parameters
                break;
            } else if(token.get_type() == TokenType::COMMA_TOKEN){ // More parameters
                pop(tokens);
            } else {
                parsing_error("Syntax error: expected ',' or ')'", token);
            }
        }
    }


    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEPAR_TOKEN){
        parsing_error("Syntax error: expected ')'", token);
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::OPENBRACKET_TOKEN){
        parsing_error("Syntax error: expected '{'", token);
    }

    // check if there are statements inside the function
    if(peek(tokens).get_type() == TokenType::CLOSEBRACKET_TOKEN){
        pop(tokens);
        return; // Empty function
    }


    Node* stmt_list = new Node(NodeType::STMT_LIST_NODE, "", token.get_line_number());
    function->add_child(stmt_list);
    parse_stmt_list(tokens, stmt_list);

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
        parsing_error("Syntax error: expected '}'", token);
    }
}

/*
Parses a vector; Ex: let a = [1, 2, 3];
Can have any value type inside the vector as well as mixing types
*/
void parse_list(std::vector<Token>& tokens, Node* current){
    //check if the list is empty
    if(peek(tokens).get_type() == TokenType::CLOSESQUAREBRACKET_TOKEN){
        pop(tokens);
        return;
    }

    bool prev_was_comma = false;
    for(;;){
        Token token = peek(tokens);
        if(token.get_type() == TokenType::CLOSESQUAREBRACKET_TOKEN){
            if(prev_was_comma){
                parsing_error("Syntax error: expected expression", token);
            }
            pop(tokens);
            break;
        }
        if(token.get_type() == TokenType::COMMA_TOKEN){
            if(prev_was_comma){
                parsing_error("Syntax error: expected expression", token);
            }
            pop(tokens);
            prev_was_comma = true;
            continue;
        }

        parse_value(tokens, current);
        prev_was_comma = false;
    }
}

/*
Parse a map; Ex: let m = map { let a = 1; let b = 2; };
Can have any value type inside the map as well as mixing types
*/
void parse_map(std::vector<Token>& tokens, Node* map_node){
    // check for opening bracket
    Token token = pop(tokens);
    if(token.get_type() != TokenType::OPENBRACKET_TOKEN){
        parsing_error("Syntax error: expected '{'", token);
    }

    // Parse the fields
    Node* list = new Node(NodeType::LIST_NODE, "", token.get_line_number());
    map_node->add_child(list);
    token = peek(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){ // Check if there are fields
        for(;;){ // Can have 0 or more let statments 
            // if the tokenb is a close bracket, then we are done with the map (but there was an extra comma)
            token = peek(tokens);
            if(token.get_type() == TokenType::CLOSEBRACKET_TOKEN){
                parsing_error("Syntax error: expected another key-value pair", token);
                break;
            }

            // Create an assignment node (Will be of a different format than a normal assignment)
            Node* assignment = new Node(NodeType::ASSIGN_NODE, "inner_map", token.get_line_number());
            list->add_child(assignment);

            // Parse the key
            parse_value(tokens, assignment);

            token = pop(tokens);
            if(token.get_type() != TokenType::COLON_TOKEN){
                parsing_error("Syntax error: expected ':'", token);
            }

            // Parse the value
            parse_value(tokens, assignment);

            token = peek(tokens);
            if(token.get_type() == TokenType::CLOSEBRACKET_TOKEN){ // End of fields
                break;
            } else if(token.get_type() == TokenType::COMMA_TOKEN){ // More fields
                pop(tokens);
            } else {
                parsing_error("Syntax error: expected ',' or '}'", token);
            }
        }
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
        parsing_error("Syntax error: expected '}'", token);
    }
}

/*
Parses an assignment; Ex: let a = 5;
*/
void parse_assignment(std::vector<Token>& tokens, Node* current, bool is_const /* = false */){
    std::string keyword = is_const ? "const" : "let";
    Node* assign = new Node(NodeType::ASSIGN_NODE, keyword, peek(tokens).get_line_number());
    current->add_child(assign);

    Token token = pop(tokens);
    if(token.get_type() != TokenType::IDENTIFIER_TOKEN){ 
        parsing_error("Syntax error: expected identifier", token);
    } 
    Node* var = new Node(NodeType::VAR_NODE, token.get_value(), token.get_line_number()); 
    assign->add_child(var);

    token = pop(tokens);
    if(token.get_type() != TokenType::ASSIGNMENT_TOKEN){ 
        parsing_error("Syntax error: expected assignment operator", token);  
    } 

    parse_value(tokens, assign);

    token = pop(tokens);
    if(token.get_type() != TokenType::SEMICOLON_TOKEN){
        parsing_error("Syntax error: expected ';'", token);
    }
}

/*
Parses an if, else if, else block; Ex: if (a < 5) { stmt_list } else if (a == 5) { stmt_list } else { stmt_list }
It is optional to have else if and else blocks
Can have any number of else if blocks
*/
void parse_if(std::vector<Token>& tokens, Node* current){
    // parse if block
    Node* if_node = new Node(NodeType::IF_NODE, "", peek(tokens).get_line_number());
    current->add_child(if_node);

    Token token = pop(tokens);
    if(token.get_value() != "("){
        parsing_error("Syntax error: expected '('", token);
    }

    // Parse the condition
    Node* expr = new Node(NodeType::EXPR_NODE, "", token.get_line_number());
    if_node->add_child(expr);
    parse_expr(tokens, expr);

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEPAR_TOKEN){
        parsing_error("Syntax error: expected ')'", token);
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::OPENBRACKET_TOKEN){
        parsing_error("Syntax error: expected '{'", token);
    }


    Node* stmt_list = new Node(NodeType::STMT_LIST_NODE, "", token.get_line_number());
    if_node->add_child(stmt_list);
    if(peek(tokens).get_type() != TokenType::CLOSEBRACKET_TOKEN){ // Check if there are statements inside the if block
        parse_stmt_list(tokens, stmt_list);
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
        parsing_error("Syntax error: expected '}'", token);
    }

    // parse else if blocks (if they exist)
    while(peek(tokens).get_type() == TokenType::ELSE_IF_TOKEN){
        pop(tokens); // Skip the 'else if' keyword
        if(pop(tokens).get_value() != "("){
            parsing_error("Syntax error: expected '('", token);
        }

        // Parse the condition
        Node* expr = new Node(NodeType::EXPR_NODE, "", token.get_line_number());
        if_node->add_child(expr);
        parse_expr(tokens, expr);

        token = pop(tokens);
        if(token.get_type() != TokenType::CLOSEPAR_TOKEN){
            parsing_error("Syntax error: expected ')'", token);
        }

        token = pop(tokens);
        if(token.get_type() != TokenType::OPENBRACKET_TOKEN){
            parsing_error("Syntax error: expected '{'", token);
        }

        Node* stmt_list = new Node(NodeType::STMT_LIST_NODE, "", token.get_line_number());
        if_node->add_child(stmt_list);
        if(peek(tokens).get_type() != TokenType::CLOSEBRACKET_TOKEN){ // Check if there are statements inside the if block
            parse_stmt_list(tokens, stmt_list);
        }

        token = pop(tokens);
        if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
            parsing_error("Syntax error: expected '}'", token);
        }
    }

    // parse else block (if it exists)
    if(peek(tokens).get_type() == TokenType::ELSE_TOKEN){
        pop(tokens); // Skip the 'else' keyword
        token = pop(tokens);
        if(token.get_type() != TokenType::OPENBRACKET_TOKEN){
            parsing_error("Syntax error: expected '{'", token);
        }

        Node* stmt_list = new Node(NodeType::STMT_LIST_NODE, "", token.get_line_number());
        if_node->add_child(stmt_list);
        if(peek(tokens).get_type() != TokenType::CLOSEBRACKET_TOKEN){ // Check if there are statements inside the if block
            parse_stmt_list(tokens, stmt_list);
        }

        token = pop(tokens);
        if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
            parsing_error("Syntax error: expected '}'", token);
        }
    }
}

/*
Parses a return statement; Ex: return 5;
Any value type can be returned
*/
void parse_return(std::vector<Token>& tokens, Node* current){
    Node* return_node = new Node(NodeType::RETURN_NODE, "", peek(tokens).get_line_number());
    current->add_child(return_node);

    // Expression to return
    parse_value(tokens, return_node);

    Token token = pop(tokens);
    if(token.get_type() != TokenType::SEMICOLON_TOKEN){
        parsing_error("Syntax error: expected ';'", token);
    }
}

/*
Parses an accessor chain; Ex: a[0][1];
*/
void parse_accessor(std::vector<Token>& tokens, Node* current){
    Node* expr = new Node(NodeType::EXPR_NODE, "", peek(tokens).get_line_number());
    current->add_child(expr);
    parse_expr(tokens, expr);
    Token token = pop(tokens);
    if(token.get_type() != TokenType::CLOSESQUAREBRACKET_TOKEN){
        parsing_error("Syntax error: expected ']'", token);
    }

    if(peek(tokens).get_value() == "["){ // Check if there is another accessor
        pop(tokens);
        parse_accessor(tokens, current);
    }
}

/*
Parses a variable update; Ex: a = 5;
Any value type can be assigned
*/
void parse_variable_update(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    if(token.get_type() != TokenType::IDENTIFIER_TOKEN){
        parsing_error("Syntax error: expected identifier", token);
    }

    Node* update = new Node(NodeType::UPDATE_NODE, "", token.get_line_number());
    current->add_child(update);

    Node* var = new Node(NodeType::VAR_NODE, token.get_value(), token.get_line_number()); // Variable to update
    update->add_child(var);
   
    if(peek(tokens).get_value() == "["){
        pop(tokens);
        parse_accessor(tokens, var);
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::ASSIGNMENT_TOKEN){
        parsing_error("Syntax error: expected assignment operator", token);
    }

    parse_value(tokens, update);
}

/*
Parses a print statement; Ex: print 5;
Any value type can be printed
*/
void parse_print(std::vector<Token>& tokens, Node* current){
    Node* print = new Node(NodeType::PRINT_NODE, "", peek(tokens).get_line_number());
    current->add_child(print);

    parse_value(tokens, print);

    Token token = pop(tokens);
    if(token.get_type() != TokenType::SEMICOLON_TOKEN){
        parsing_error("Syntax error: expected ';'", token);
    }
}

/*
Parses a for loop; Ex: for (let i = 0; i < 5; i = i + 1) { stmt_list };
First statement is the initialization, second is the condition (expr), and third is the variable update, they must be these stmt types
*/
void parse_for(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    Node* for_node = new Node(NodeType::FOR_NODE, "", token.get_line_number());
    current->add_child(for_node);
    if(token.get_value() != "("){
        parsing_error("Syntax error: expected '('", token);
    }

    // Parse the initialization 
    if(peek(tokens).get_type() != TokenType::SEMICOLON_TOKEN){ // Check if there is an initialization
        pop(tokens); // Skip the let keyword(usally done in the parse_stmt function)
        parse_assignment(tokens, for_node);
    }
    else{
        pop(tokens); // Skip the semicolon
    }

    // Parse the condition
    if(peek(tokens).get_type() != TokenType::SEMICOLON_TOKEN){ // Check if there is a condition
        Node* condition = new Node(NodeType::EXPR_NODE, "", token.get_line_number());
        for_node->add_child(condition);
        parse_expr(tokens, condition);
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::SEMICOLON_TOKEN){
        parsing_error("Syntax error: expected ';'", token);
    }

    // Parse the variable update 
    if(peek(tokens).get_type() != TokenType::CLOSEPAR_TOKEN){ // Check if there is a variable update
        parse_variable_update(tokens, for_node);    
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEPAR_TOKEN){
        parsing_error("Syntax error: expected ')'", token);
    }

    // for body 
    token = pop(tokens);
    if(token.get_type() != TokenType::OPENBRACKET_TOKEN){
        parsing_error("Syntax error: expected '{'", token);
    }

    // Check if there are statements inside the for block
    if(peek(tokens).get_type() == TokenType::CLOSEBRACKET_TOKEN){
        pop(tokens);
        return; // Empty for block
    }

    // Parse for block
    Node* stmt_list = new Node(NodeType::STMT_LIST_NODE, "", token.get_line_number());
    for_node->add_child(stmt_list);
    parse_stmt_list(tokens, stmt_list);

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
        parsing_error("Syntax error: expected '}'", token);
    }
}

/*
Parses a foreach loop; Ex: foreach (let key : value in list) { stmt_list };
First statement is the key, second is the value, and third is the vector or map, they must be these stmt types
*/
void parse_foreach(std::vector<Token>& tokens, Node* current){
    Node* foreach_node = new Node(NodeType::FOREACH_NODE, "", peek(tokens).get_line_number());
    current->add_child(foreach_node);
    
    Token token = pop(tokens);    
    if(token.get_value() != "("){
        parsing_error("Syntax error: expected '('", token);
    }

    // let keyword
    token = pop(tokens);
    if(token.get_type() != TokenType::LET_TOKEN){
        parsing_error("Syntax error: expected 'let'", token);
    }

    // key
    token = pop(tokens);
    if(token.get_type() != TokenType::IDENTIFIER_TOKEN){
        parsing_error("Syntax error: expected identifier", token);
    }
    Node* key = new Node(NodeType::VAR_NODE, token.get_value(), token.get_line_number());
    foreach_node->add_child(key);

    // :
    token = pop(tokens);
    if(token.get_type() != TokenType::COLON_TOKEN){
        parsing_error("Syntax error: expected ':'", token);
    }

    // value
    token = pop(tokens);
    if(token.get_type() != TokenType::IDENTIFIER_TOKEN){
        parsing_error("Syntax error: expected identifier", token);
    }
    Node* value = new Node(NodeType::VAR_NODE, token.get_value(), token.get_line_number());
    foreach_node->add_child(value);

    // in
    token = pop(tokens);
    if(token.get_type() != TokenType::IN_TOKEN){
        parsing_error("Syntax error: expected 'in'", token);
    }

    // list
    parse_value(tokens, foreach_node);

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEPAR_TOKEN){
        parsing_error("Syntax error: expected ')'", token);
    }

    // foreach body
    token = pop(tokens);
    if(token.get_type() != TokenType::OPENBRACKET_TOKEN){
        parsing_error("Syntax error: expected '{'", token);
    }

    // Check if there are statements inside the foreach block
    if(peek(tokens).get_type() == TokenType::CLOSEBRACKET_TOKEN){
        pop(tokens);
        return; // Empty foreach block
    }

    // Parse foreach block
    Node* stmt_list = new Node(NodeType::STMT_LIST_NODE, "", token.get_line_number());
    foreach_node->add_child(stmt_list);
    parse_stmt_list(tokens, stmt_list);

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
        parsing_error("Syntax error: expected '}'", token);
    }
}

/*
Parses a define statement; Ex: define op in type as func
where op, type, and func are expressions
*/
void parse_define(std::vector<Token>& tokens, Node* current){
    Node* define_node = new Node(NodeType::DEFINE_NODE, "", peek(tokens).get_line_number());
    current->add_child(define_node);

    // parse expr (op)
    parse_value(tokens, define_node);

    // check for in keyword
    Token token = pop(tokens);
    if(token.get_type() != TokenType::IN_TOKEN){
        parsing_error("Syntax error: expected 'in'", token);
    }

    // parse expr (type)
    parse_value(tokens, define_node);

    // check for the as keyword
    token = pop(tokens);
    if(token.get_type() != TokenType::AS_TOKEN){
        parsing_error("Syntax error: expected 'as'", token);
    }

    // parse expr
    parse_value(tokens, define_node);

    // check for semicolon
    token = pop(tokens);
    if(token.get_type() != TokenType::SEMICOLON_TOKEN){
        parsing_error("Syntax error: expected ';'", token);
    }
}

/*
Starting point for parsing a statement
*/
void parse_stmt(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    switch(token.get_type()){
        case TokenType::EOF_TOKEN:
            parsing_error("Syntax error: expected statement", token);
        case TokenType::IF_TOKEN:
            parse_if(tokens, current);
            break;
        case TokenType::RETURN_TOKEN:
            parse_return(tokens, current);
            break;
        case TokenType::LET_TOKEN:
            parse_assignment(tokens, current);
            break;
        case TokenType::CONST_TOKEN:
            parse_assignment(tokens, current, true);
            break;
        case TokenType::DEFINE_TOKEN:
            parse_define(tokens, current);
            break;
        case TokenType::PRINT_TOKEN:
            parse_print(tokens, current);
            break;
        case TokenType::IDENTIFIER_TOKEN:
            place_token_back(tokens, token);
            parse_variable_update(tokens, current);
            // Check if the statement ends with a semicolon
            // Have to check here because the parse_variable_update function does not check for it because
            // it is also used in for loops and didn't want the semicolon to be mandatory there
            token = pop(tokens);
            if(token.get_type() != TokenType::SEMICOLON_TOKEN){
                parsing_error("Syntax error: expected ';'", token);
            }
            break;
        case TokenType::FOR_TOKEN:
            parse_for(tokens, current);
            break;
        case TokenType::FOREACH_TOKEN:
            parse_foreach(tokens, current);
            break;
        case TokenType::CONTINUE_TOKEN:
        {
            token = pop(tokens);
            if(token.get_type() != TokenType::SEMICOLON_TOKEN){
                parsing_error("Syntax error: expected ';'", token);
            }
            Node* continue_node = new Node(NodeType::CONTINUE_NODE, "", token.get_line_number());
            current->add_child(continue_node);
        }
            break;
        case TokenType::BREAK_TOKEN:
        {
            token = pop(tokens);
            if(token.get_type() != TokenType::SEMICOLON_TOKEN){
                parsing_error("Syntax error: expected ';'", token);
            }
            Node* break_node = new Node(NodeType::BREAK_NODE, "", token.get_line_number());
            current->add_child(break_node);
        }
            break;
        default:
            parsing_error("Syntax error: expected statement", token);
    }
}

/*
Starting point for parsing a list of statements
*/
void parse_stmt_list(std::vector<Token>& tokens, Node* current){
    Token token = peek(tokens);
    if(token.get_type() == TokenType::CLOSEBRACKET_TOKEN){ // End of block (function, if, for, etc)
        return;
    }

    Node* stmt = new Node(NodeType::STMT_NODE, "", token.get_line_number());
    current->add_child(stmt);
    parse_stmt(tokens, stmt);

    token = peek(tokens);
    if(token.get_type() == TokenType::EOF_TOKEN){
        return;
    }

    Node* stmt_list = new Node(NodeType::STMT_LIST_NODE, "", token.get_line_number());
    current->add_child(stmt_list); 
    parse_stmt_list(tokens, stmt_list);
}

/*
Entry point for the parser
Give it a list of tokens and it will return the root node of the AST
*/
Node* parse(std::vector<Token> tokens, bool verbose = false){
    Node* root = new Node(NodeType::STMT_LIST_NODE, "", -1);
    ROOT_NODE = root;
    parse_stmt_list(tokens, root);

    // Print the AST
    if(verbose){
        std::cout << "Printing the AST..." << std::endl;
        root->print();
    }

    return root;
}

// -------------------------------------------------------------------

#endif // PARSER_HPP