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

    // OTHERS
    LIST_NODE,
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
        case NodeType::LIST_NODE:
            return "LIST";
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

int precedence(std::string op){ // TODO: Could be speed up with a map
    if(op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!="){
        return 7;
    }
    else if(op == "+" || op == "-"){
        return 9;
    } 
    else if(op == "*" || op == "/"){
        return 10;
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
void parse_expr(std::vector<Token>& tokens, Node* current, bool nested); 
void parse_function_call(std::vector<Token>& tokens, Node* current);
void parse_std_lib_call(std::vector<Token>& tokens, Node* current);
void parse_function(std::vector<Token>& tokens, Node* current);
void parse_assignment(std::vector<Token>& tokens, Node* current);
void parse_if(std::vector<Token>& tokens, Node* current);
void parse_return(std::vector<Token>& tokens, Node* current);
void parse_variable_update(std::vector<Token>& tokens, Node* current);
void parse_print(std::vector<Token>& tokens, Node* current);

void parse_expr(std::vector<Token>& tokens, Node* current, bool nested = false){
    std::stack<Node*> ops;
    std::stack<Node*> values;

    bool loop = true;

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
                    Node* var = new Node(NodeType::VAR_NODE, value);
                    values.push(var);
                }
                break;
            }
            case TokenType::OPERATOR_TOKEN: {
                Node* op = new Node(NodeType::OP_NODE, value);
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

// This is called when the let keyword is encountered
void parse_assignment(std::vector<Token>& tokens, Node* current){
    Node* assign = new Node(NodeType::ASSIGN_NODE, "");
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
    pop(tokens); // Skip the let keyword(usally done in the parse_stmt function)
    parse_assignment(tokens, for_node);

    // Parse the condition
    Node* condition = new Node(NodeType::EXPR_NODE, "");
    for_node->add_child(condition);
    parse_expr(tokens, condition);

    token = pop(tokens);
    if(token.get_type() != TokenType::SEMICOLON_TOKEN){
        parsing_error("Syntax error: expected ';'", token);
    }

    // Parse the variable update 
    parse_variable_update(tokens, for_node);

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