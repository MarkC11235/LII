#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>

#include "tokenizer.hpp"

enum NodeType {
    PROGRAM,
    FUNCTION,
    STMT_LIST,
    STMT,
    WHILE,
    FOR,
    ASSIGN,
    UPDATE,
    EXPR,
    IF,
    RETURN,
    TERM,
    FACTOR,
    VAR,
    NUM,
    OP,
    LIST
};


// Helper functions ---------------------------------------------------
int parsing_error(std::string message, Token token){
    std::cout << message << std::endl;
    std::cout << "Token: " << token.get_value() << " at line " << token.get_line_number() << std::endl;
    exit(1);
}

std::string node_type_to_string(NodeType type){
    switch(type){
        case NodeType::PROGRAM:
            return "PROGRAM";
        case NodeType::FUNCTION:
            return "FUNCTION";
        case NodeType::STMT_LIST:
            return "STMT_LIST";
        case NodeType::STMT:
            return "STMT";
        case NodeType::WHILE:
            return "WHILE";
        case NodeType::FOR:
            return "FOR";
        case NodeType::ASSIGN:
            return "ASSIGN";
        case NodeType::UPDATE:
            return "UPDATE";
        case NodeType::EXPR:
            return "EXPR";
        case NodeType::IF:
            return "IF";
        case NodeType::RETURN:
            return "RETURN";
        case NodeType::TERM:
            return "TERM";
        case NodeType::FACTOR:
            return "FACTOR";
        case NodeType::VAR:
            return "VAR";
        case NodeType::NUM:
            return "NUM";
        case NodeType::OP:
            return "OP";
        case NodeType::LIST:
            return "LIST";
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

int precedence(std::string op){
    if(op == "+" || op == "-"){
        return 1;
    } else if(op == "*" || op == "/"){
        return 2;
    }
    return 0;
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
void parse_expr(std::vector<Token>& tokens, Node* current); 

void parse_expr(std::vector<Token>& tokens, Node* current){
    std::stack<Node*> ops;
    std::stack<Node*> values;

    for(;;){
        Token token = peek(tokens);
        if((token.get_type() != TokenType::NUMBER_TOKEN 
            && token.get_type() != TokenType::OPERATOR_TOKEN
            && token.get_type() != TokenType::IDENTIFIER_TOKEN)
            || token.get_type() == TokenType::EOF_TOKEN){
            break;
        }
        token = pop(tokens);

        if(token.get_type() == TokenType::NUMBER_TOKEN){
            Node* num = new Node(NodeType::NUM, token.get_value());
            values.push(num);
        } 
        else if(token.get_type() == TokenType::IDENTIFIER_TOKEN){
            Node* var = new Node(NodeType::VAR, token.get_value());
            values.push(var);
        }
        else if(token.get_type() == TokenType::OPERATOR_TOKEN){
            Node* op = new Node(NodeType::OP, token.get_value());
            while(ops.size() > 0 && precedence(ops.top()->get_value()) >= precedence(op->get_value())){
                Node* top = ops.top();
                ops.pop();
                top->add_child(values.top());
                values.pop();
                top->add_child(values.top());
                values.pop();
                values.push(top);
            }
            ops.push(op);
        }
    }

    while(ops.size() > 0){
        Node* top = ops.top();
        ops.pop();
        top->add_child(values.top());
        values.pop();
        top->add_child(values.top());
        values.pop();
        values.push(top);
    }

    current->add_child(values.top());
}

void parse_assignment(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    Node* assign = new Node(NodeType::ASSIGN, "");
    current->add_child(assign);

    if(token.get_type() == TokenType::IDENTIFIER_TOKEN){
        Node* var = new Node(NodeType::VAR, token.get_value());
        assign->add_child(var);
        token = pop(tokens);

        if(token.get_type() == TokenType::ASSIGNMENT_TOKEN){
            Node* expr = new Node(NodeType::EXPR, "");
            assign->add_child(expr);
            parse_expr(tokens, expr);
        } else {
            parsing_error("Syntax error: expected assignment operator", token);
        }
    } else {
        parsing_error("Syntax error: expected identifier", token);
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::SEMICOLON_TOKEN){
        parsing_error("Syntax error: expected ';'", token);
    }
}

void parse_if(std::vector<Token>& tokens, Node* current){
    Node* if_node = new Node(NodeType::IF, "");
    current->add_child(if_node);

    Token token = pop(tokens);
    if(token.get_type() != TokenType::OPENPAR_TOKEN){
        parsing_error("Syntax error: expected '('", token);
    }

    Node* expr = new Node(NodeType::EXPR, "");
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

    if(peek(tokens).get_type() != TokenType::CLOSEBRACKET_TOKEN){ // Check if there are statements inside the if block
        Node* stmt_list = new Node(NodeType::STMT_LIST, "");
        if_node->add_child(stmt_list);
        parse_stmt_list(tokens, stmt_list);
    }

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
        parsing_error("Syntax error: expected '}'", token);
    }

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
    Node* stmt_list = new Node(NodeType::STMT_LIST, "");
    if_node->add_child(stmt_list);
    parse_stmt_list(tokens, stmt_list);

    token = pop(tokens);
    if(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
        parsing_error("Syntax error: expected '}'", token); ////////////// HERE
    }
    
}

void parse_return(std::vector<Token>& tokens, Node* current){
    Node* return_node = new Node(NodeType::RETURN, "");
    current->add_child(return_node);

    Node* expr = new Node(NodeType::EXPR, "");
    return_node->add_child(expr);
    parse_expr(tokens, expr);

    Token token = pop(tokens);
    if(token.get_type() != TokenType::SEMICOLON_TOKEN){
        parsing_error("Syntax error: expected ';'", token);
    }
}

void parse_variable_update(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    if(token.get_type() != TokenType::IDENTIFIER_TOKEN){
        parsing_error("Syntax error: expected identifier", token);
    }

    Node* update = new Node(NodeType::UPDATE, "");
    current->add_child(update);

    Node* var = new Node(NodeType::VAR, token.get_value());
    update->add_child(var);

    token = pop(tokens);
    if(token.get_type() != TokenType::ASSIGNMENT_TOKEN){
        parsing_error("Syntax error: expected assignment operator", token);
    }

    Node* expr = new Node(NodeType::EXPR, "");
    update->add_child(expr);
    parse_expr(tokens, expr);

    token = pop(tokens);
    if(token.get_type() != TokenType::SEMICOLON_TOKEN){
        parsing_error("Syntax error: expected ';'", token);
    }
}

void parse_stmt(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    if(token.get_type() == TokenType::EOF_TOKEN){
        return; // End of file
    }
    if(token.get_type() == TokenType::IF_TOKEN){
        parse_if(tokens, current);
    } 
    else if(token.get_type() == TokenType::RETURN_TOKEN){
        parse_return(tokens, current);
    }
    else if(token.get_type() == TokenType::LET_TOKEN){
        parse_assignment(tokens, current);
    }
    else if(token.get_type() == TokenType::IDENTIFIER_TOKEN){
        place_token_back(tokens, token);
        parse_variable_update(tokens, current);
    }
    else {
        parsing_error("Syntax error: expected statement", token);
    }
}

void parse_stmt_list(std::vector<Token>& tokens, Node* current){
    Token token = peek(tokens);
    if(token.get_type() == TokenType::CLOSEBRACKET_TOKEN){ // End of block
        return;
    }

    Node* stmt = new Node(NodeType::STMT, "");
    current->add_child(stmt);
    parse_stmt(tokens, stmt);

    token = peek(tokens);
    if(token.get_type() == TokenType::EOF_TOKEN){
        return;
    }

    Node* stmt_list = new Node(NodeType::STMT_LIST, "");
    current->add_child(stmt_list); 
    parse_stmt_list(tokens, stmt_list);
}

Node* parse(std::vector<Token> tokens, bool verbose = false){
    Node* root = new Node(NodeType::STMT_LIST, "");
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