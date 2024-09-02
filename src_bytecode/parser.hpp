#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>

#include "./std_lib/std_lib.hpp"
#include "tokenizer.hpp"

enum NodeType {
    // PROGRAM
    PROGRAM_NODE,
    FUNCTION_NODE,
    STD_LIB_CALL_NODE,
    STMT_LIST_NODE,
    STMT_NODE,
    // CONTROL FLOW
    WHILE_NODE,
    FOR_NODE,
    IF_NODE,
    RETURN_NODE,
    BREAK_NODE,
    CONTINUE_NODE,
    // ASSIGNMENTS
    ASSIGN_NODE,
    UPDATE_NODE,
    // ARITHMETIC
    EXPR_NODE,
    TERM_NODE,
    FACTOR_NODE,
    VAR_NODE,
    NUM_NODE,
    OP_NODE,
    STRING_NODE,
    BOOL_NODE,
    NULL_NODE,

    // OTHERS
    LIST_NODE,
    STRUCT_NODE,
    PRINT_NODE,
    FUNCTION_CALL_NODE
};


// Helper functions ---------------------------------------------------
int parsing_error(std::string message, Token token){
    std::cout << message << std::endl;
    std::cout << "Token: " << token.get_value() << " at line " << token.get_line_number() << std::endl;
    exit(1);
}

std::string node_type_to_string(NodeType type){
    switch(type){
        case NodeType::PROGRAM_NODE:
            return "PROGRAM";
        case NodeType::FUNCTION_NODE:
            return "FUNCTION";
        case NodeType::STD_LIB_CALL_NODE:
            return "STD_LIB_CALL";
        case NodeType::STMT_LIST_NODE:
            return "STMT_LIST";
        case NodeType::STMT_NODE:
            return "STMT";
        case NodeType::WHILE_NODE:
            return "WHILE";
        case NodeType::FOR_NODE:
            return "FOR";
        case NodeType::IF_NODE:
            return "IF";
        case NodeType::RETURN_NODE:
            return "RETURN";
        case NodeType::BREAK_NODE:
            return "BREAK";
        case NodeType::CONTINUE_NODE:
            return "CONTINUE";
        case NodeType::ASSIGN_NODE:
            return "ASSIGN";
        case NodeType::UPDATE_NODE:
            return "UPDATE";
        case NodeType::EXPR_NODE:
            return "EXPR";
        case NodeType::TERM_NODE:
            return "TERM";
        case NodeType::FACTOR_NODE:
            return "FACTOR";
        case NodeType::VAR_NODE:
            return "VAR";
        case NodeType::NUM_NODE:
            return "NUM";
        case NodeType::OP_NODE:
            return "OP";
        case NodeType::STRING_NODE:
            return "STRING";
        case NodeType::BOOL_NODE:   
            return "BOOL";
        case NodeType::NULL_NODE:
            return "NULL";
        case NodeType::LIST_NODE:
            return "LIST";
        case NodeType::STRUCT_NODE:
            return "STRUCT";
        case NodeType::PRINT_NODE:
            return "PRINT";
        case NodeType::FUNCTION_CALL_NODE:
            return "FUNCTION_CALL";
    }
    return "UNKNOWN";
}

Token peek(std::vector<Token> tokens){
    if(tokens.size() > 0){
        return tokens[0];
    } else {
        return Token(TokenType::EOF_TOKEN, "EOF", -1);
    }
}

Token pop(std::vector<Token>& tokens){
    if(tokens.size() > 0){
        Token token = tokens[0];
        tokens.erase(tokens.begin());
        return token;
    } else {
        return Token(TokenType::EOF_TOKEN, "EOF", -1);
    }
}

void place_token_back(std::vector<Token>& tokens, Token token){
    tokens.insert(tokens.begin(), token);
}

std::map<std::string, std::tuple<int, std::string>> operators = {
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

// Data structures ---------------------------------------------------

class Node {
    NodeType type;
    std::vector<std::string> values;
    std::vector<Node*> children;
public:
    Node(NodeType type, std::vector<std::string> values){
        this->type = type;
        this->values = values;
    }
    Node(NodeType type, std::string value){
        this->type = type;
        this->values.push_back(value);
    }
    ~Node(){
        for(int i = 0; i < int(this->children.size()); i++){
            delete this->children[i];
        }

        this->children.clear();

        this->values.clear();
    }
    void add_child(Node* child){
        this->children.push_back(child);
    }
    void add_value(std::string value){
        this->values.push_back(value);
    }
    void change_value(std::string value, int index){
        this->values[index] = value;
    }
    NodeType get_type(){
        return this->type;
    }
    std::string get_value(int index = 0){
        return this->values[index];
    }
    std::vector<std::string> get_values(){
        return this->values;
    }
    std::vector<Node*> get_children(){
        return this->children;
    }
    Node* get_child(int index){
        return this->children[index];
    }

    void print(int level = 0){
        for(int i = 0; i < level; i++){
            std::cout << "  ";
        }
        std::cout << node_type_to_string(this->get_type()) << " ";
        for(int i = 0; i < int(this->values.size()); i++){
            std::cout << this->values[i] << " ";
        }
        std::cout << std::endl;
        for(int i = 0; i < int(this->children.size()); i++){
            this->children[i]->print(level + 1);
        }
    }
};

// -------------------------------------------------------------------

// Parsing functions -------------------------------------------------

// Forward declarations
void parse_stmt_list(std::vector<Token>& tokens, Node* current);
void parse_stmt(std::vector<Token>& tokens, Node* current);
void parse_expr(std::vector<Token>& tokens, Node* current, bool nested); 
void parse_function_call(std::vector<Token>& tokens, Node* current);
void parse_std_lib_call(std::vector<Token>& tokens, Node* current);
void parse_function(std::vector<Token>& tokens, Node* current);
void parse_assignment(std::vector<Token>& tokens, Node* current, bool is_const = false);
void parse_if(std::vector<Token>& tokens, Node* current);
void parse_return(std::vector<Token>& tokens, Node* current);
void parse_list(std::vector<Token>& tokens, Node* current, int level);
void parse_struct(std::vector<Token>& tokens, Node* current);
void parse_accessor(std::vector<Token>& tokens, Node* current);
void parse_variable_update(std::vector<Token>& tokens, Node* current);
void parse_print(std::vector<Token>& tokens, Node* current);

void parse_expr(std::vector<Token>& tokens, Node* current, bool nested = false){
    std::stack<Node*> ops;
    std::stack<Node*> values;

    bool loop = true;

    TokenType previos_token_type = TokenType::OPERATOR_TOKEN;

    while(loop){
        Token token = pop(tokens);
        if(token.get_type() == TokenType::CLOSEPAR_TOKEN && nested){ // End of nested expression
            break;
        }

        TokenType type = token.get_type();
        std::string value = token.get_value();

        switch(type){
            case TokenType::NUMBER_TOKEN: {
                Node* num = new Node(NodeType::NUM_NODE, value);
                values.push(num);
                break;
            }
            case TokenType::BOOL_TOKEN: {
                Node* bool_node = new Node(NodeType::BOOL_NODE, value);
                values.push(bool_node);
                break;
            }
            case TokenType::NULL_TOKEN: {
                parsing_error("Syntax error: null cannot be used in expressions", token);
                break;
            }
            case TokenType::STD_LIB_TOKEN: {
                Node* std_lib = new Node(NodeType::STD_LIB_CALL_NODE, value);
                parse_std_lib_call(tokens, std_lib);
                values.push(std_lib);
                break;
            }
            case TokenType::IDENTIFIER_TOKEN: {
                if(peek(tokens).get_type() == TokenType::OPENPAR_TOKEN){ // Function call
                    place_token_back(tokens, token);
                    Node* function_call = new Node(NodeType::FUNCTION_CALL_NODE, "");
                    parse_function_call(tokens, function_call);
                    values.push(function_call);
                }
                else if(peek(tokens).get_type() == TokenType::OPENSQUAREBRACKET_TOKEN){ // Array access
                    pop(tokens); // Skip the '['
                    Node* var = new Node(NodeType::VAR_NODE, value);
                    Node* index = new Node(NodeType::EXPR_NODE, "");
                    var->add_child(index);
                    parse_expr(tokens, index);

                    values.push(var);

                    // check for closing bracket
                    Token token = pop(tokens);
                    if(token.get_type() != TokenType::CLOSESQUAREBRACKET_TOKEN){
                        parsing_error("Syntax error: expected ']'", token);
                    }
                }
                else { // Variable
                    //check if the identifier is an accessor
                    if(peek(tokens).get_type() == TokenType::ACCESSOR_TOKEN){
                        Node* var = new Node(NodeType::VAR_NODE, value);
                        parse_accessor(tokens, var);
                        values.push(var);
                    }
                    else {
                        Node* var = new Node(NodeType::VAR_NODE, value);
                        values.push(var);
                    }
                }
                break;
            }
            case TokenType::OPERATOR_TOKEN: {
                Node* op = new Node(NodeType::OP_NODE, value);

                if(previos_token_type == TokenType::OPERATOR_TOKEN){
                    if(value == "-"){ // Unary minus
                        op->change_value("u-", 0);
                    }
                    else if(value == "!"){ // not
                        op->change_value("!", 0); // keeps it the same, just here so it does not go to the else
                    }
                    else {
                        parsing_error("Syntax error: expected number or identifier", token);
                    }
                }

                while(ops.size() > 0 && (precedence(ops.top()->get_value(), token) >= precedence(op->get_value(), token)) ){
                    Node* top = ops.top();
                    ops.pop();
                    if(is_binary_operator(top->get_value())){
                        top->add_child(values.top());
                        values.pop();
                        top->add_child(values.top());
                        values.pop();
                    }
                    else if(is_unary_operator(top->get_value())){
                        top->add_child(values.top());
                        values.pop();
                    }
                    else {
                        parsing_error("Syntax error: unknown operator", token);
                    }
                    values.push(top);
                }

                ops.push(op);
                break;
            }
            case TokenType::OPENPAR_TOKEN: {
                Node* expr = new Node(NodeType::EXPR_NODE, "");
                parse_expr(tokens, expr, true); // Nested expression
                values.push(expr);
                break;
            }
            case TokenType::STRING_TOKEN: {
                Node* str = new Node(NodeType::STRING_NODE, value);
                values.push(str);
                break;
            }
            default: {
                place_token_back(tokens, token);
                loop = false;
                break;
            }
        }

        previos_token_type = type;
    }

    while(ops.size() > 0){
        Node* top = ops.top();
        ops.pop();
        if(is_binary_operator(top->get_value())){
            top->add_child(values.top());
            values.pop();
            top->add_child(values.top());
            values.pop();
            values.push(top);
        }
        else if(is_unary_operator(top->get_value())){
            top->add_child(values.top());
            values.pop();
            values.push(top);
        }
    }

    if(values.size() != 1){
        parsing_error("Syntax error: invalid expression", Token(TokenType::EOF_TOKEN, "EOF", -1));
    }
    current->add_child(values.top());
}

void parse_function_call(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    if(token.get_type() != TokenType::IDENTIFIER_TOKEN){
        parsing_error("Syntax error: expected identifier", token);
    }

    current->add_value(token.get_value());

    token = pop(tokens);
    if(token.get_type() != TokenType::OPENPAR_TOKEN){
        parsing_error("Syntax error: expected '('", token);
    }

    // Parse the parameters
    Node* list = new Node(NodeType::LIST_NODE, "");
    current->add_child(list);
    token = peek(tokens);
    if(token.get_type() != TokenType::CLOSEPAR_TOKEN){ // Check if there are parameters
        for(;;){ // Can have 0 or more parameters
            Node* expr = new Node(NodeType::EXPR_NODE, "");
            list->add_child(expr);
            parse_expr(tokens, expr);

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

void parse_std_lib_call(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    if(token.get_type() != TokenType::IDENTIFIER_TOKEN){
        parsing_error("Syntax error: expected identifier", token);
    }

    current->add_value(token.get_value());

    token = pop(tokens);
    if(token.get_type() != TokenType::OPENPAR_TOKEN){
        parsing_error("Syntax error: expected '('", token);
    }

    // Parse the parameters
    Node* list = new Node(NodeType::LIST_NODE, "");
    current->add_child(list);
    token = peek(tokens);
    if(token.get_type() != TokenType::CLOSEPAR_TOKEN){ // Check if there are parameters
        for(;;){ // Can have 0 or more parameters
            Node* expr = new Node(NodeType::EXPR_NODE, "");
            list->add_child(expr);
            parse_expr(tokens, expr);

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

void parse_function(std::vector<Token>& tokens, Node* current){
    pop(tokens); // Skip the 'func' keyword
    Node* function = new Node(NodeType::FUNCTION_NODE, "");
    current->add_child(function);

    // check for opening parenthesis
    Token token = pop(tokens);
    if(token.get_type() != TokenType::OPENPAR_TOKEN){
        parsing_error("Syntax error: expected '('", token);
    }

    // Parse the parameters
    Node* list = new Node(NodeType::LIST_NODE, "");
    function->add_child(list);
    token = peek(tokens);
    if(token.get_type() != TokenType::CLOSEPAR_TOKEN){ // Check if there are parameters
        for(;;){ // Can have 0 or more parameters
            token = pop(tokens);
            if(token.get_type() == TokenType::IDENTIFIER_TOKEN){ // Identifier
                Node* var = new Node(NodeType::VAR_NODE, token.get_value());
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

    Node* stmt_list = new Node(NodeType::STMT_LIST_NODE, "");
    function->add_child(stmt_list);
    parse_stmt_list(tokens, stmt_list);

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
        parsing_error("Syntax error: expected '}'", token);
    }
}

void parse_list(std::vector<Token>& tokens, Node* current, int level = 0){
    pop(tokens); // Skip the '['
    Node* list = new Node(NodeType::LIST_NODE, "");
    current->add_child(list);
    //check if the list is empty
    if(peek(tokens).get_type() == TokenType::CLOSESQUAREBRACKET_TOKEN){
        pop(tokens);
        return;
    }

    // Parse the elements, allow nested lists
    bool prev_was_comma = false;
    for(;;){
        Token token = peek(tokens);
        if(token.get_type() == TokenType::CLOSESQUAREBRACKET_TOKEN){
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

        if(token.get_type() == TokenType::OPENSQUAREBRACKET_TOKEN){
            Node* nested_list = new Node(NodeType::LIST_NODE, "");
            list->add_child(nested_list);
            parse_list(tokens, nested_list, level + 1);
        } 
        else {
            Node* expr = new Node(NodeType::EXPR_NODE, "");
            list->add_child(expr);
            parse_expr(tokens, expr);
        }
        prev_was_comma = false;
    }
}

void parse_struct(std::vector<Token>& tokens, Node* current){
    pop(tokens); // Skip the 'struct' keyword
    Node* struct_node = new Node(NodeType::STRUCT_NODE, "");
    current->add_child(struct_node);

    // check for opening bracket
    Token token = pop(tokens);
    if(token.get_type() != TokenType::OPENBRACKET_TOKEN){
        parsing_error("Syntax error: expected '{'", token);
    }

    // Parse the fields
    Node* list = new Node(NodeType::LIST_NODE, "");
    struct_node->add_child(list);
    token = peek(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){ // Check if there are fields
        for(;;){ // Can have 0 or more let statments 
            token = pop(tokens);
            if(token.get_type() == TokenType::LET_TOKEN){ // Let statement
                parse_assignment(tokens, list);
            } else {
                parsing_error("Syntax error: expected 'let'", token);
            }

            token = peek(tokens);
            if(token.get_type() == TokenType::CLOSEBRACKET_TOKEN){ // End of fields
                break;
            }
        }
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
        parsing_error("Syntax error: expected '}'", token);
    }
}

// This is called when the let keyword is encountered
void parse_assignment(std::vector<Token>& tokens, Node* current, bool is_const /* = false */){
    std::string keyword = is_const ? "const" : "let";
    Node* assign = new Node(NodeType::ASSIGN_NODE, keyword);
    current->add_child(assign);

    Token token = pop(tokens);
    if(token.get_type() != TokenType::IDENTIFIER_TOKEN){ 
        parsing_error("Syntax error: expected identifier", token);
    } 
    Node* var = new Node(NodeType::VAR_NODE, token.get_value()); 
    assign->add_child(var);

    token = pop(tokens);
    if(token.get_type() != TokenType::ASSIGNMENT_TOKEN){ 
        parsing_error("Syntax error: expected assignment operator", token);  
    } 

    //Find type of assignment
    if(peek(tokens).get_type() == TokenType::OPENSQUAREBRACKET_TOKEN){ // Array assignment
        parse_list(tokens, assign);
    }
    else if(peek(tokens).get_type() == TokenType::FUNC_TOKEN){ // Function assignment
        parse_function(tokens, assign);
    }
    else if(peek(tokens).get_type() == TokenType::NULL_TOKEN){ // Null assignment
        pop(tokens);
        Node* null_node = new Node(NodeType::NULL_NODE, "null");
        assign->add_child(null_node);
    }
    else if (peek(tokens).get_type() == TokenType::STRUCT_TOKEN){ // Struct assignment
        parse_struct(tokens, assign);
    }
    else{ // Expression assignment
        Node* expr = new Node(NodeType::EXPR_NODE, "");
        assign->add_child(expr);
        parse_expr(tokens, expr);
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::SEMICOLON_TOKEN){
        parsing_error("Syntax error: expected ';'", token);
    }
}

void parse_if(std::vector<Token>& tokens, Node* current){
    Node* if_node = new Node(NodeType::IF_NODE, "");
    current->add_child(if_node);

    Token token = pop(tokens);
    if(token.get_type() != TokenType::OPENPAR_TOKEN){
        parsing_error("Syntax error: expected '('", token);
    }

    // Parse the condition
    Node* expr = new Node(NodeType::EXPR_NODE, "");
    if_node->add_child(expr);
    parse_expr(tokens, expr);

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEPAR_TOKEN){
        parsing_error("Syntax error: expected ')'", token);
    }

    // if block 
    token = pop(tokens);
    if(token.get_type() != TokenType::OPENBRACKET_TOKEN){
        parsing_error("Syntax error: expected '{'", token);
    }

    if(peek(tokens).get_type() != TokenType::CLOSEBRACKET_TOKEN){ // Check if there are statements inside the if block
        Node* stmt_list = new Node(NodeType::STMT_LIST_NODE, "");
        if_node->add_child(stmt_list);
        parse_stmt_list(tokens, stmt_list);
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
        parsing_error("Syntax error: expected '}'", token);
    }


    // else block 
    // Check for else
    token = peek(tokens);
    if(token.get_type() != TokenType::ELSE_TOKEN){
        return; // No else block
    }

    // Parse else
    pop(tokens);

    token = pop(tokens);
    if(token.get_type() != TokenType::OPENBRACKET_TOKEN){
        parsing_error("Syntax error: expected '{'", token);
    }

    // Check if there are statements inside the else block
    if(peek(tokens).get_type() == TokenType::CLOSEBRACKET_TOKEN){
        pop(tokens);
        return; // Empty else block
    }

    // Parse else block
    Node* stmt_list = new Node(NodeType::STMT_LIST_NODE, "");
    if_node->add_child(stmt_list);
    parse_stmt_list(tokens, stmt_list);

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
        parsing_error("Syntax error: expected '}'", token); 
    }

}

void parse_return(std::vector<Token>& tokens, Node* current){
    Node* return_node = new Node(NodeType::RETURN_NODE, "");
    current->add_child(return_node);

    // Expression to return
    Node* expr = new Node(NodeType::EXPR_NODE, "");
    return_node->add_child(expr);
    parse_expr(tokens, expr);

    Token token = pop(tokens);
    if(token.get_type() != TokenType::SEMICOLON_TOKEN){
        parsing_error("Syntax error: expected ';'", token);
    }
}

// Recursive function to handle nested accessors
void parse_accessor(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens); // Skip the dot
    token = pop(tokens);
    if(token.get_type() != TokenType::IDENTIFIER_TOKEN){
        parsing_error("Syntax error: expected identifier", token);
    }
    Node* field = new Node(NodeType::VAR_NODE, token.get_value());
    current->add_child(field);

    token = peek(tokens);
    if(token.get_type() == TokenType::ACCESSOR_TOKEN){
        parse_accessor(tokens, field);
    }
}

// This is called when an identifier is encountered, with no let keyword
void parse_variable_update(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    if(token.get_type() != TokenType::IDENTIFIER_TOKEN){
        parsing_error("Syntax error: expected identifier", token);
    }

    Node* update = new Node(NodeType::UPDATE_NODE, "");
    current->add_child(update);

    Node* var = new Node(NodeType::VAR_NODE, token.get_value()); // Variable to update
    update->add_child(var);

    token = pop(tokens);
    if(token.get_type() == TokenType::OPENSQUAREBRACKET_TOKEN){ // Array update
        Node* index = new Node(NodeType::EXPR_NODE, ""); // Index
        update->add_child(index);
        parse_expr(tokens, index); // Index
        token = pop(tokens);
        if(token.get_type() != TokenType::CLOSESQUAREBRACKET_TOKEN){
            parsing_error("Syntax error: expected ']'", token);
        }

        token = pop(tokens);
    }
    else if(token.get_type() == TokenType::ACCESSOR_TOKEN){ // Struct update
        place_token_back(tokens, token);
        parse_accessor(tokens, var);
        token = pop(tokens);
    }

    if(token.get_type() != TokenType::ASSIGNMENT_TOKEN){
        parsing_error("Syntax error: expected assignment operator", token);
    }

    Node* expr = new Node(NodeType::EXPR_NODE, ""); // Expression to assign
    update->add_child(expr);
    parse_expr(tokens, expr);
}

void parse_print(std::vector<Token>& tokens, Node* current){
    Node* print = new Node(NodeType::PRINT_NODE, "");
    current->add_child(print);

    Node* expr = new Node(NodeType::EXPR_NODE, ""); // Expression to print
    print->add_child(expr);
    parse_expr(tokens, expr);

    Token token = pop(tokens);
    if(token.get_type() != TokenType::SEMICOLON_TOKEN){
        parsing_error("Syntax error: expected ';'", token);
    }
}

void parse_for(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    Node* for_node = new Node(NodeType::FOR_NODE, "");
    current->add_child(for_node);
    if(token.get_type() != TokenType::OPENPAR_TOKEN){
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
        Node* condition = new Node(NodeType::EXPR_NODE, "");
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
    Node* stmt_list = new Node(NodeType::STMT_LIST_NODE, "");
    for_node->add_child(stmt_list);
    parse_stmt_list(tokens, stmt_list);

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
        parsing_error("Syntax error: expected '}'", token);
    }
}

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
        case TokenType::CONTINUE_TOKEN:
        {
            token = pop(tokens);
            if(token.get_type() != TokenType::SEMICOLON_TOKEN){
                parsing_error("Syntax error: expected ';'", token);
            }
            Node* continue_node = new Node(NodeType::CONTINUE_NODE, "");
            current->add_child(continue_node);
        }
            break;
        case TokenType::BREAK_TOKEN:
        {
            token = pop(tokens);
            if(token.get_type() != TokenType::SEMICOLON_TOKEN){
                parsing_error("Syntax error: expected ';'", token);
            }
            Node* break_node = new Node(NodeType::BREAK_NODE, "");
            current->add_child(break_node);
        }
            break;
        case TokenType::STD_LIB_TOKEN:
            place_token_back(tokens, token); // parse_expr expects the std lib token
            parse_expr(tokens, current);
            token = pop(tokens);
            if(token.get_type() != TokenType::SEMICOLON_TOKEN){
                parsing_error("Syntax error: expected ';'", token);
            }
            break;
        default:
            parsing_error("Syntax error: expected statement", token);
    }
}

void parse_stmt_list(std::vector<Token>& tokens, Node* current){
    Token token = peek(tokens);
    if(token.get_type() == TokenType::CLOSEBRACKET_TOKEN){ // End of block (function, if, for, etc)
        return;
    }

    Node* stmt = new Node(NodeType::STMT_NODE, "");
    current->add_child(stmt);
    parse_stmt(tokens, stmt);

    token = peek(tokens);
    if(token.get_type() == TokenType::EOF_TOKEN){
        return;
    }

    Node* stmt_list = new Node(NodeType::STMT_LIST_NODE, "");
    current->add_child(stmt_list); 
    parse_stmt_list(tokens, stmt_list);
}

Node* parse(std::vector<Token> tokens, bool verbose = false){
    // TODO: make stmt_list be able to be empty, so the grammar matches the language
    Node* root = new Node(NodeType::STMT_LIST_NODE, "");
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