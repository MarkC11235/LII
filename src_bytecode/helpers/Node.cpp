#include "Node.hpp"

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
        case NodeType::FOREACH_NODE:
            return "FOREACH";
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
        case NodeType::DEFINE_NODE:
            return "DEFINE";
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
        case NodeType::MAP_NODE:
            return "MAP";
        case NodeType::PRINT_NODE:
            return "PRINT";
        case NodeType::FUNCTION_CALL_NODE:
            return "FUNCTION_CALL";
        case NodeType::ERROR_NODE:
            return "ERROR";
    }
    return "UNKNOWN";
}

Node::Node(NodeType type, std::vector<std::string> values, int line_number){
    this->type = type;
    this->values = values;
    this->line_number = line_number;
}

Node::Node(NodeType type, std::string value, int line_number){
    this->type = type;
    this->values.push_back(value);
    this->line_number = line_number;
}

Node::~Node(){
    for(int i = 0; i < int(this->children.size()); i++){
        delete this->children[i];
    }

    this->children.clear();

    this->values.clear();
}

void Node::add_child(Node* child){
    this->children.push_back(child);
}

void Node::add_value(std::string value){
    this->values.push_back(value);
}

void Node::change_value(std::string value, int index){
    this->values[index] = value;
}

NodeType Node::get_type(){
    return this->type;
}

std::string Node::get_value(int index){
    return this->values[index];
}

std::vector<std::string> Node::get_values(){
    return this->values;
}

std::vector<Node*> Node::get_children(){
    return this->children;
}

Node* Node::get_child(int index){
    return this->children[index];
}

int Node::get_line_number(){
    return this->line_number;
}

void Node::print(int level){
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

std::string Node::to_string(int level){
    std::string str = "";
    for(int i = 0; i < level; i++){
        str += "  ";
    }
    str += node_type_to_string(this->get_type()) + " ";
    for(int i = 0; i < int(this->values.size()); i++){
        str += this->values[i] + " ";
    }
    str += "\n";
    for(int i = 0; i < int(this->children.size()); i++){
        str += this->children[i]->to_string(level + 1);
    }
    return str;
}

