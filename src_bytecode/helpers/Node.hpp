#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <vector>
#include <string>

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
    FOREACH_NODE,
    IF_NODE,
    RETURN_NODE,
    BREAK_NODE,
    CONTINUE_NODE,
    // ASSIGNMENTS
    ASSIGN_NODE,
    UPDATE_NODE,
    DEFINE_NODE,
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
    MAP_NODE,
    PRINT_NODE,
    FUNCTION_CALL_NODE
};

std::string node_type_to_string(NodeType type);

class Node {
private:
    NodeType type;
    std::vector<std::string> values;
    std::vector<Node*> children;
    int line_number;

public:
    Node(NodeType type, std::vector<std::string> values, int line_number);
    Node(NodeType type, std::string value, int line_number);
    ~Node();
    
    void add_child(Node* child);
    void add_value(std::string value);
    void change_value(std::string value, int index);
    NodeType get_type();
    std::string get_value(int index = 0);
    std::vector<std::string> get_values();
    std::vector<Node*> get_children();
    Node* get_child(int index);
    int get_line_number();
    void print(int level = 0);
};

#endif // NODE_HPP