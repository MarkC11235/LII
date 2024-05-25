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

int parsing_error(std::string message, Token token){
    std::cout << message << std::endl;
    std::cout << "Token: " << token.get_value() << " at line " << token.get_line_number() << std::endl;
    // exit(1);
    return 1;
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

void parse_stmt_list(std::vector<Token>& tokens, Node* current);
void parse_stmt(std::vector<Token>& tokens, Node* current);

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

void parse_expr(std::vector<Token>& tokens, Node* current);

void parse_function_call(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    Node* function_call = new Node(NodeType::FUNCTION, token.get_value());
    current->add_child(function_call);
    token = pop(tokens);

    if(token.get_type() == TokenType::OPENPAR_TOKEN){
        token = pop(tokens);
        while(token.get_type() != TokenType::CLOSEPAR_TOKEN){
            if(token.get_type() == TokenType::NUMBER_TOKEN){
                Node* num = new Node(NodeType::NUM, token.get_value());
                function_call->add_child(num);
            } else if(token.get_type() == TokenType::IDENTIFIER_TOKEN){
                Node* var = new Node(NodeType::VAR, token.get_value());
                function_call->add_child(var);
            }
            token = pop(tokens);
        }
    } else {
        parsing_error("Syntax error: expected opening parenthesis", token);
    }
}

int precedence(std::string op){
    if(op == "+" || op == "-"){
        return 1;
    } else if(op == "*" || op == "/"){
        return 2;
    }
    return 0;
}

void parse_expr(std::vector<Token>& tokens, Node* current){
    std::stack<Node*> ops;
    std::stack<Node*> values;

    for(;;){
        //std::cout << "Tokens left: " << tokens.size() << std::endl;
        Token token = peek(tokens);
        if((token.get_type() != TokenType::NUMBER_TOKEN && token.get_type() != TokenType::OPERATOR_TOKEN) || token.get_type() == TokenType::EOF_TOKEN){
            break;
        }
        token = pop(tokens);

        // print stack
        // std::stack<Node*> temp = ops;
        // std::cout << "Op Stack: ";
        // while(temp.size() > 0){
        //     std::cout << temp.top()->get_value() << " ";
        //     temp.pop();
        // }
        // std::cout << std::endl;

        // //print values
        // temp = values;
        // std::cout << "Value Stack: ";
        // while(temp.size() > 0){
        //     // std::cout << temp.top()->get_value() << " ";
        //     // temp.pop();
        //     temp.top()->print();
        //     temp.pop();
        // }
        // std::cout << std::endl << std::endl;

        if(token.get_type() == TokenType::NUMBER_TOKEN){
            Node* num = new Node(NodeType::NUM, token.get_value());
            values.push(num);
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
    if(token.get_type() == TokenType::EOF_TOKEN){
        return;
    }
    if(token.get_type() == TokenType::IDENTIFIER_TOKEN){
        Node* var = new Node(NodeType::VAR, token.get_value());
        current->add_child(var);
        token = pop(tokens);
        if(token.get_type() == TokenType::ASSIGNMENT_TOKEN){
            Node* assign = new Node(NodeType::OP, "=");
            current->add_child(assign);

            Node* expr = new Node(NodeType::EXPR, "");
            current->add_child(expr);
            parse_expr(tokens, expr);
        } else {
            parsing_error("Syntax error: expected assignment operator", token);
        }
    } else {
        parsing_error("Syntax error: expected identifier", token);
    }
}

void parse_if(std::vector<Token>& tokens, Node* current){
    Node* if_node = new Node(NodeType::IF, "");
    current->add_child(if_node);
    Token token = pop(tokens);
    if(token.get_type() == TokenType::OPENPAR_TOKEN){
        Node* expr = new Node(NodeType::EXPR, "");
        if_node->add_child(expr);
        parse_expr(tokens, expr);

        token = pop(tokens);
        if(token.get_type() == TokenType::CLOSEPAR_TOKEN){
            token = pop(tokens);
            if(token.get_type() == TokenType::OPENBRACKET_TOKEN){
                 Node* stmt_list = new Node(NodeType::STMT_LIST, "");
                if_node->add_child(stmt_list);
                parse_stmt_list(tokens, stmt_list);

                token = pop(tokens);
                if(token.get_type() == TokenType::CLOSEBRACKET_TOKEN){
                    token = pop(tokens);
                    if(token.get_type() == TokenType::ELSE_TOKEN){
                        token = pop(tokens);
                        if(token.get_type() == TokenType::OPENBRACKET_TOKEN){
                            Node* else_stmt_list = new Node(NodeType::STMT_LIST, "");
                            if_node->add_child(else_stmt_list);
                            parse_stmt_list(tokens, else_stmt_list);

                            token = pop(tokens);
                            if(token.get_type() == TokenType::CLOSEBRACKET_TOKEN){
                                return;
                            } else {
                                parsing_error("Syntax error: expected closing bracket", token);
                            }
                        } else {
                            parsing_error("Syntax error: expected opening bracket", token);
                        }
                    } else {
                        place_token_back(tokens, token);
                        return;
                    }
                } else {
                    parsing_error("Syntax error: expected closing bracket", token);
                }
            } else {
                parsing_error("Syntax error: expected opening bracket", token);
            }
        } else {
            parsing_error("Syntax error: expected closing parenthesis", token);
        }
    } else {
        parsing_error("Syntax error: expected opening parenthesis", token);
    }
}

void parse_while(std::vector<Token>& tokens, Node* current){
    Node* while_node = new Node(NodeType::WHILE, "");
    current->add_child(while_node);
    Token token = pop(tokens);
    if(token.get_type() == TokenType::OPENPAR_TOKEN){
        Node* expr = new Node(NodeType::EXPR, "");
        while_node->add_child(expr);
        parse_expr(tokens, expr);

        token = pop(tokens);
        if(token.get_type() == TokenType::CLOSEPAR_TOKEN){
            token = pop(tokens);
            if(token.get_type() == TokenType::OPENBRACKET_TOKEN){
                Node* stmt_list = new Node(NodeType::STMT_LIST, "");
                while_node->add_child(stmt_list);
                parse_stmt_list(tokens, stmt_list);

                token = pop(tokens);
                if(token.get_type() == TokenType::CLOSEBRACKET_TOKEN){
                    return;
                } else {
                    parsing_error("Syntax error: expected closing bracket", token);
                }
            } else {
                parsing_error("Syntax error: expected opening bracket", token);
            }
        } else {
            parsing_error("Syntax error: expected closing parenthesis", token);
        }
    } else {
        parsing_error("Syntax error: expected opening parenthesis", token);
    }
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

void parse_for(std::vector<Token>& tokens, Node* current, std::string directive = ""){
    std::vector<std::string> directives = splitStringByComma(directive);
    if(directives.size() == 0){
        directives.push_back("");
    }
    Node* for_node = new Node(NodeType::FOR, directives);
    
    current->add_child(for_node);
    Token token = pop(tokens);
    if(token.get_type() == TokenType::OPENPAR_TOKEN){
        Node* assign = new Node(NodeType::STMT, "");
        for_node->add_child(assign);
        parse_stmt(tokens, assign);

        token = pop(tokens);
        if(token.get_type() == TokenType::SEMICOLON_TOKEN){
            Node* expr = new Node(NodeType::STMT, "");
            for_node->add_child(expr);
            parse_stmt(tokens, expr);

            token = pop(tokens);
            if(token.get_type() == TokenType::SEMICOLON_TOKEN){
                Node* assign = new Node(NodeType::STMT, "");
                for_node->add_child(assign);
                parse_stmt(tokens, assign);

                token = pop(tokens);
                if(token.get_type() == TokenType::CLOSEPAR_TOKEN){
                    token = pop(tokens);
                    if(token.get_type() == TokenType::OPENBRACKET_TOKEN){
                        Node* stmt_list = new Node(NodeType::STMT_LIST, "");
                        for_node->add_child(stmt_list);
                        parse_stmt_list(tokens, stmt_list);

                        token = pop(tokens);
                        if(token.get_type() == TokenType::CLOSEBRACKET_TOKEN){
                            return;
                        } else {
                            parsing_error("Syntax error: expected closing bracket", token);
                        }
                    } else {
                        parsing_error("Syntax error: expected opening bracket", token);
                    }
                } else {
                    parsing_error("Syntax error: expected closing parenthesis", token);
                }
            } else {
                parsing_error("Syntax error: expected semicolon", token);
            }
        } else {
            parsing_error("Syntax error: expected semicolon", token);
        }
    } else {
        parsing_error("Syntax error: expected opening parenthesis", token);
    }
}

void parse_return(std::vector<Token>& tokens, Node* current){
    Node* return_node = new Node(NodeType::RETURN, "");
    current->add_child(return_node);

    //Node* expr = new Node(NodeType::EXPR, "");
    //return_node->add_child(expr);
    //parse_expr(tokens, expr);
}

void parse_list(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    //token.print();
    if(token.get_type() == TokenType::EOF_TOKEN){
        return;
    }
    if(token.get_type() == TokenType::OPENBRACKET_TOKEN){
        //std::cout << "Parsing list" << std::endl;
        token = pop(tokens);
        while(token.get_type() != TokenType::CLOSEBRACKET_TOKEN){
            if(token.get_type() == TokenType::NUMBER_TOKEN){
                Node* num = new Node(NodeType::NUM, token.get_value());
                current->add_child(num);
            } else if(token.get_type() == TokenType::IDENTIFIER_TOKEN){
                Node* var = new Node(NodeType::VAR, token.get_value());
                current->add_child(var);
            }
            token = pop(tokens);
        }
    } else {
        parsing_error("Syntax error: expected opening bracket", token);
    }
}

void parse_array_assign(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    if(token.get_type() == TokenType::EOF_TOKEN){
        return;
    }
    Node* assign = new Node(NodeType::ASSIGN, "");
    current->add_child(assign);
    if(token.get_type() == TokenType::IDENTIFIER_TOKEN){
        Node* var = new Node(NodeType::VAR, token.get_value());
        assign->add_child(var);
        token = pop(tokens);

        if(token.get_type() == TokenType::OPENSQUAREBRACKET_TOKEN){
            Node* expr = new Node(NodeType::EXPR, "");
            assign->add_child(expr);
            parse_expr(tokens, expr);

            token = pop(tokens);
            if(token.get_type() == TokenType::CLOSESQUAREBRACKET_TOKEN){
                token = pop(tokens);
                if(token.get_type() == TokenType::ASSIGNMENT_TOKEN){
                    Node* a = new Node(NodeType::OP, "=");
                    assign->add_child(a);

                    if(peek(tokens).get_type() == TokenType::OPENBRACKET_TOKEN){
                        Node* list = new Node(NodeType::LIST, "");
                        assign->add_child(list);
                        parse_list(tokens, list);
                    } else if(peek(tokens).get_type() == TokenType::NUMBER_TOKEN || peek(tokens).get_type() == TokenType::IDENTIFIER_TOKEN){
                        Node* expr = new Node(NodeType::EXPR, "");
                        assign->add_child(expr);
                        parse_expr(tokens, expr);
                    } else {
                        parsing_error("Syntax error: expected list or expression", token);
                    }

                } else {
                    parsing_error("Syntax error: expected assignment operator", token);
                }
            } else {
                parsing_error("Syntax error: expected closing square bracket", token);
            }
        } else {
            parsing_error("Syntax error: expected opening square bracket", token);
        }
    }
}

void parse_stmt(std::vector<Token>& tokens, Node* current){
    Token token = pop(tokens);
    if(token.get_type() == TokenType::EOF_TOKEN){
        return;
    }
    if(token.get_type() == TokenType::IF_TOKEN){
        parse_if(tokens, current);
    } 
    else if(token.get_type() == TokenType::WHILE_TOKEN){
        parse_while(tokens, current);
    }
    else if(token.get_type() == TokenType::DIRECTIVE_TOKEN){
        std::string directive = token.get_value().substr(2);
        pop(tokens);
        parse_for(tokens, current, directive);
    }
    else if(token.get_type() == TokenType::FOR_TOKEN){
        parse_for(tokens, current);
    }
    else if(token.get_type() == TokenType::RETURN_TOKEN){
        parse_return(tokens, current);
    }
    else if(token.get_type() == TokenType::IDENTIFIER_TOKEN || token.get_type() == TokenType::NUMBER_TOKEN){
        if(peek(tokens).get_type() == TokenType::ASSIGNMENT_TOKEN){
            Node* assign = new Node(NodeType::ASSIGN, "");
            current->add_child(assign);
            place_token_back(tokens, token);
            parse_assignment(tokens, assign);
        } 
        else if(token.get_value() == "array" && peek(tokens).get_type() == TokenType::IDENTIFIER_TOKEN){
            parse_array_assign(tokens, current);
        }
        else {
            std::vector<Token> temp_tokens;
            temp_tokens.push_back(token);
            while(peek(tokens).get_type() != TokenType::CLOSESQUAREBRACKET_TOKEN && peek(tokens).get_type() != TokenType::EOF_TOKEN){
                token = pop(tokens);
                temp_tokens.push_back(token);
            }
            if(peek(tokens).get_type() == TokenType::ASSIGNMENT_TOKEN){
                Node* assign = new Node(NodeType::ASSIGN, "");
                current->add_child(assign);
                for(int i = 0; i < int(temp_tokens.size()); i++){
                    place_token_back(tokens, temp_tokens[i]);
                }
                parse_assignment(tokens, assign);
            } else {
                Node* expr = new Node(NodeType::EXPR, "");
                current->add_child(expr);
                token = pop(tokens);
                place_token_back(tokens, token);
                for(int i = int(temp_tokens.size()) - 1; i >= 0; i--){
                    //temp_tokens[i].print();
                    place_token_back(tokens, temp_tokens[i]);
                }
                parse_expr(tokens, expr);
            }
        }
    } 
}

void parse_stmt_list(std::vector<Token>& tokens, Node* current){
    Token token = peek(tokens);
    if(token.get_type() == TokenType::EOF_TOKEN){
        return;
    }

    if(token.get_type() == TokenType::CLOSEBRACKET_TOKEN){
        return;
    }

    Node* stmt = new Node(NodeType::STMT, "");
    current->add_child(stmt);
    parse_stmt(tokens, stmt);

    // Check to see if there are more statements
    if(peek(tokens).get_type() == TokenType::EOF_TOKEN){
        return;
    }

    Node* stmt_list = new Node(NodeType::STMT_LIST, "");
    current->add_child(stmt_list);
    parse_stmt_list(tokens, stmt_list);
}

std::vector<Node*> parse_functions(std::vector<Token>& tokens, bool verbose = false){
    std::vector<Node*> functions;
    while(tokens.size() != 0){
        Token token = pop(tokens);
        if(token.get_type() == TokenType::FUNCTION_TOKEN){
            token = pop(tokens);
            if(token.get_type() == TokenType::IDENTIFIER_TOKEN){
                Node* function = new Node(NodeType::FUNCTION, token.get_value());
                functions.push_back(function);
                token = pop(tokens);
                if(token.get_type() == TokenType::OPENPAR_TOKEN){
                    token = pop(tokens);
                    while(token.get_type() != TokenType::CLOSEPAR_TOKEN){
                        if(token.get_type() == TokenType::IDENTIFIER_TOKEN){
                            function->add_value(token.get_value());
                        }
                        token = pop(tokens);
                    }
                    token = pop(tokens);
                    if(token.get_type() == TokenType::OPENBRACKET_TOKEN){
                        Node* stmt_list = new Node(NodeType::STMT_LIST, "");
                        function->add_child(stmt_list);
                        parse_stmt_list(tokens, stmt_list);

                        token = pop(tokens);
                        if(token.get_type() == TokenType::CLOSEBRACKET_TOKEN){
                            continue;
                        } else {
                            parsing_error("Syntax error: expected closing bracket", token);
                        }
                    } else {
                        parsing_error("Syntax error: expected opening bracket", token);
                    }
                } else {
                    parsing_error("Syntax error: expected opening parenthesis", token);
                }
            } else {
                parsing_error("Syntax error: expected identifier", token);
            }
        } 
    }

    // Print the functions
    if(verbose){
        std::cout << "Printing the functions..." << std::endl;
        for(Node* function : functions){
            function->print();
        }
    }

    return functions;
}

Node* parse(std::vector<Token> tokens, bool verbose = false){
    Node* root = new Node(NodeType::PROGRAM, "");
    Node* current = new Node(NodeType::STMT_LIST, "");
    root->add_child(current);
    parse_stmt_list(tokens, current);

    // Print the AST
    if(verbose){
        std::cout << "Printing the AST..." << std::endl;
        root->print();
    }

    return root;
}

#endif // PARSER_HPP