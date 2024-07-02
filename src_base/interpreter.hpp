#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <future>

std::mutex mtx;

#include "parser.hpp"

bool speculative_exe = false;
std::map<std::string, Node*> functions_defintions;

void interpret_stmt_list(Node* node, std::map<std::string, int> &variables);
void interpret_stmt(Node* node, std::map<std::string, int> &variables);
int interpret_expr(Node* node, std::map<std::string, int> &variables);
void interpret_assign(Node* node, std::map<std::string, int> &variables);
void interpret_if(Node* node, std::map<std::string, int> &variables);
void interpret_while(Node* node, std::map<std::string, int> &variables);
void interpret_for(Node* node, std::map<std::string, int> &variables);
void interpret_return(Node* node, std::map<std::string, int> &variables);
int interpret_function(Node* node, std::map<std::string, int> &variables, std::map<std::string, Node*> &functions);
void interpret(Node* node, std::map<std::string, int> &variables, std::map<std::string, Node*> &functions, bool sif);

void interpretation_error(std::string message, Node* node){
    std::cout << message << std::endl;
    node->print();
    exit(1);
}

int interpret_factor(Node* node, std::map<std::string, int> &variables){
    if(node->get_type() != NodeType::FACTOR){
        interpretation_error("Factor doesn't start with FACTOR Node", node);
    }
    Node* child = node->get_child(0);
    if(child->get_type() == NodeType::VAR){
        if(child->get_values().size() == 2){
            std::string var_name = child->get_value();
            std::string index = child->get_value(1);
            if(variables.find(var_name + "[" + index + "]") == variables.end()){
                if(variables.find(index) == variables.end()){
                    interpretation_error("Variable not found", child);
                }
                std::string index_var = index;
                index = std::to_string(variables[index]);
                return variables[var_name + "[" + index + "]"];
            }
            return variables[var_name + "[" + index + "]"];
        }
        else{
            std::string var_name = child->get_value();
            if(variables.find(var_name) == variables.end()){
                interpretation_error("Variable not found", child);
            }
            return variables[var_name];
        }
    }
    else if(child->get_type() == NodeType::NUM){
        return std::stoi(child->get_value());
    }
    else if(child->get_type() == NodeType::FUNCTION){
        int value = interpret_function(child, variables, functions_defintions);
        //std::cout << value << std::endl;
        return value;
    }
    else{
        interpretation_error("Factor doesn't contain VAR or NUM", node);
    }

    interpretation_error("Factor not supported", node);
}

int interpret_term(Node* node, std::map<std::string, int> &variables){
    if(node->get_type() != NodeType::TERM){
        interpretation_error("Term doesn't start with TERM Node", node);
    }

    Node* child = node->get_child(0);
    if(node->get_children().size() == 1){
        return interpret_factor(child, variables);
    }
    else if(node->get_children().size() == 3){
        int left;
        if(child->get_type() == NodeType::FACTOR){
            left = interpret_factor(child, variables);
        }
        else{
            interpretation_error("Left Child of Term doesn't contain FACTOR Node", node);
        }

        Node* op = node->get_child(1);
        if(op->get_type() != NodeType::OP){
            interpretation_error("Operator Node not found", node);
        }

        Node* right_child = node->get_child(2);
        int right;
        if(right_child->get_type() == NodeType::TERM){
            right = interpret_term(right_child, variables);
        }
        else if(right_child->get_type() == NodeType::FACTOR){
            right = interpret_factor(right_child, variables);
        }
        else{
            interpretation_error("Right Child of Term doesn't contain FACTOR or TERM Node", node);
        }

        if(op->get_value() == "*"){
            return left * right;
        }
        else if(op->get_value() == "/"){
            return left / right;
        }
        else{
            interpretation_error("Operator not supported", op);
        }
    }
    else{
        interpretation_error("Term doesn't have 1 or 3 children", node);
    }

    interpretation_error("Term not supported", node);
}

int interpret_expr(Node* node, std::map<std::string, int> &variables){
    if(node->get_type() != NodeType::EXPR){
        interpretation_error("Expression doesn't start with EXPR Node", node);
    }

    Node* child = node->get_child(0);
    if(node->get_children().size() == 1){
        return interpret_term(child, variables);
    }
    else if(node->get_children().size() == 3){
        int left;
        if(child->get_type() == NodeType::TERM){
            left = interpret_term(child, variables);
        }
        else{
            interpretation_error("Left Child of Expression doesn't contain TERM Node", node);
        }

        Node* op = node->get_child(1);
        if(op->get_type() != NodeType::OP){
            interpretation_error("Operator Node not found", node);
        }

        Node* right_child = node->get_child(2);
        int right;
        if(right_child->get_type() == NodeType::EXPR){
            right = interpret_expr(right_child, variables);
        }
        else if(right_child->get_type() == NodeType::TERM){
            right = interpret_term(right_child, variables);
        }
        else if(right_child->get_type() == NodeType::FACTOR){
            right = interpret_factor(right_child, variables);
        }
        else if(right_child->get_type() == NodeType::VAR){
            std::string var_name = right_child->get_value();
            if(variables.find(var_name) == variables.end()){
                interpretation_error("Variable not found", right_child);
            }
            right = variables[var_name];
        }
        else if(right_child->get_type() == NodeType::NUM){
            right = std::stoi(right_child->get_value());
        }
        else{
            interpretation_error("Right Child of Expression doesn't contain VAR, NUM, TERM or EXPR Node", node);
        }

        if(op->get_value() == "+"){
            return left + right;
        }
        else if(op->get_value() == "-"){
            return left - right;
        }
        else if(op->get_value() == "<"){
            return left < right;
        }
        else if(op->get_value() == ">"){
            return left > right;
        }
        else if(op->get_value() == "<="){
            return left <= right;
        }
        else if(op->get_value() == ">="){
            return left >= right;
        }
        else if(op->get_value() == "=="){
            return left == right;
        }
        else if(op->get_value() == "!="){
            return left != right;
        }
        else{
            interpretation_error("Operator not supported", op);
        }
    }
    else{
        interpretation_error("Expression doesn't have 1 or 3 children", node);
    }

    interpretation_error("Expression not supported", node);
}

void interpret_assign_array(Node* node, std::map<std::string, int> &variables){
    std::string var_name = node->get_child(0)->get_value();

    if(node->get_children().size() != 4){
        interpretation_error("Assign Array doesn't have 4 children", node);
    }
    if(node->get_child(3)->get_type() == NodeType::LIST){
        std::vector<int> values;
        Node* list = node->get_child(3);
        for(int i = 0; i < (int)list->get_children().size(); i++){
            values.push_back(std::stoi(list->get_child(i)->get_value()));
        }
        for(int i = 0; i < (int)values.size(); i++){
            variables[var_name + "[" + std::to_string(i) + "]"] = values[i];
        }
        int arr_size = interpret_expr(node->get_child(1), variables);
        while((int)values.size() < arr_size){
            values.push_back(std::stoi(list->get_child(list->get_children().size() - 1)->get_value()));
        }
        for(int i = 0; i < (int)values.size(); i++){
            variables[var_name + "[" + std::to_string(i) + "]"] = values[i];
        }
        
    }
    else{
        int index = interpret_expr(node->get_child(1), variables);
        int value = interpret_expr(node->get_child(3), variables);
        // std::cout << "Assigning " << value << " to " << var_name + "[" + std::to_string(index) + "]" << std::endl;
        variables[var_name + "[" + std::to_string(index) + "]"] = value;
        // for(auto &var : variables){
        //     std::cout << var.first << " = " << var.second << std::endl;
        // }
    }
}

void interpret_assign(Node* node, std::map<std::string, int> &variables){
    if(node->get_children().size() == 3){
        std::string var_name = node->get_child(0)->get_value();
        int value = interpret_expr(node->get_child(2), variables); // var = expr, so expr is the third child
        variables[var_name] = value;
    }
    else if(node->get_children().size() == 4){
        interpret_assign_array(node, variables);
    }
    else{
        interpretation_error("Assign doesn't have 3 or 4 children", node);
    }
}

void interpret_if_speculation(Node* node, std::map<std::string, int> &variables){
    // speculative if execution
    // runs the if and else branches in parallel and chooses the correct one
    // also runs the condition in parallel
    std::map<std::string, int> new_variables = variables;
    std::future<int> f1 = std::async(interpret_expr, node->get_child(0), std::ref(new_variables)); // Condition
    std::thread t_if(interpret_stmt_list, node->get_child(1), std::ref(new_variables)); // Body
    if(node->get_children().size() == 2){
        f1.get();
        t_if.join();
        variables = new_variables;
        return;
    }

    std::map<std::string, int> new_variables2 = variables;  
    std::thread t_else(interpret_stmt_list, node->get_child(2), std::ref(new_variables2)); // Else

    t_if.join();
    t_else.join();
    int value = f1.get();
    if(value != 0){
        variables = new_variables;
    }
    else{
        variables = new_variables2;
    }
}

void interpret_if(Node* node, std::map<std::string, int> &variables){
    //speculative if execution
    if(speculative_exe){
        interpret_if_speculation(node, variables);
        return;
    }

    int value = interpret_expr(node->get_child(0), variables);
    if(value != 0){
        interpret_stmt_list(node->get_child(1), variables);
    }
    else{
        if(node->get_children().size() == 3){
            interpret_stmt_list(node->get_child(2), variables);
        }
    }
}

void interpret_while(Node* node, std::map<std::string, int> &variables){
    int value = interpret_expr(node->get_child(0), variables);
    while(value != 0){
        interpret_stmt_list(node->get_child(1), variables);
        value = interpret_expr(node->get_child(0), variables);
    }
}

void interpret_for_body(Node* node, std::map<std::string, int> &variables){
    interpret_stmt_list(node->get_child(3), variables); // Body
    mtx.lock();
    interpret_stmt(node->get_child(2), variables); // Update
    mtx.unlock();
}

void interpret_for_thread(Node* node, std::map<std::string, int> &variables, int start, int end, int num_threads){
    //std::cout << "Interpreting for loop with " << start << " to " << end << std::endl;
    for(int i = start; i < end; i += num_threads){
        mtx.lock();
        variables[node->get_value(0)] = i;
        mtx.unlock();
        interpret_for_body(node, variables);
    }
}

void interpret_for_threaded(Node* node, std::map<std::string, int> &variables){
    //std::cout << "Interpreting for loop with n threads " << std::endl;
    int num_threads = std::stoi(node->get_value(0));
    //int start = std::stoi(node->get_value(1)); //rn assume that start is always 0
    int end = std::stoi(node->get_value(2));

    interpret_stmt(node->get_child(0), variables); // Initialize

    std::vector<std::thread> threads;
    for(int i = 0; i < num_threads; i++){
        std::cout << "Creating thread " << i << std::endl;
        threads.push_back(std::thread(interpret_for_thread, node, std::ref(variables), i, end, num_threads));
    }
    for(auto &thread : threads){
        thread.join();
    }
}

void interpret_for(Node* node, std::map<std::string, int> &variables){
    //std::cout << "Interpreting for loop" << std::endl;
    //node->print();
    if(node->get_value() == ""){
        interpret_stmt(node->get_child(0), variables); // Initialize
        int value = interpret_expr(node->get_child(1)->get_child(0), variables); // Condition
        while(value != 0){
            interpret_stmt_list(node->get_child(3), variables); // Body
            interpret_stmt(node->get_child(2), variables); // Update
            value = interpret_expr(node->get_child(1)->get_child(0), variables); // Condition
        }
    }
    else{
        interpret_for_threaded(node, variables);
    }
}

void interpret_return(Node* node, std::map<std::string, int> &variables){
    int value = interpret_expr(node->get_child(0), variables);
    //std::cout << value << std::endl;
    //exit(0);
    variables["return_value"] = value;
}

void interpret_stmt(Node* node, std::map<std::string, int> &variables){
    if(node->get_type() == NodeType::STMT){
        Node* child = node->get_child(0);
        if(child->get_type() == NodeType::ASSIGN){
            interpret_assign(child, variables);
        }
        else if(child->get_type() == NodeType::IF){
            interpret_if(child, variables);
        }
        else if(child->get_type() == NodeType::WHILE){
            interpret_while(child, variables);
        }
        else if(child->get_type() == NodeType::FOR){
            interpret_for(child, variables);
        }
        else if(child->get_type() == NodeType::RETURN){
            interpret_return(child, variables);
        }
        else if(child->get_type() == NodeType::EXPR){
            int value = interpret_expr(child, variables);
            std::cout << value << std::endl;
        }
        else{
            interpretation_error("Statement not supported", node);
        }
    }
    else{
        interpretation_error("Statement doesn't start with STMT Node", node);
    }
}

void interpret_stmt_list(Node* node, std::map<std::string, int> &variables){
    if(node->get_children().size() == 0){
        return;
    }

    if(node->get_type() != NodeType::STMT_LIST){
        interpretation_error("Statement List doesn't start with STMT_LIST Node", node);
    }

    interpret_stmt(node->get_child(0), variables);
    if(node->get_children().size() == 2){
        interpret_stmt_list(node->get_child(1), variables);
    }
}

int interpret_function(Node* node, std::map<std::string, int> &variables, std::map<std::string, Node*> &functions){
    // std::cout << "Interpreting function " << node->get_value() << std::endl;
    //New Variables for Function
    std::map<std::string, int> func_variables;
    // Arguments
    // std::cout << "Arguments" << std::endl;
    std::vector<std::string> arguments;
    for(int i = 0; i < (int)node->get_children().size(); i++){
        if(node->get_child(i)->get_type() == NodeType::VAR){
            arguments.push_back(std::to_string(variables[node->get_child(i)->get_value()]));
            // std::cout << node->get_child(i)->get_value() << " = " << variables[node->get_child(i)->get_value()] << std::endl;
        }
        else if(node->get_child(i)->get_type() == NodeType::NUM){
            arguments.push_back(node->get_child(i)->get_value());
            // std::cout << node->get_child(i)->get_value() << std::endl;
        }
        else{
            interpretation_error("Argument not supported", node->get_child(i));
        }
    }

    // Get Function Node
    // std::cout << "Function Node: ";
    // std::cout << node->get_value() << std::endl;

    //Print all functions
    // for(auto &func : functions){
    //     std::cout << func.first << std::endl;
    // }

    Node* function_node = functions[node->get_value()];

    // Assign Arguments to Variables
    for(int i = 0; i < (int)arguments.size(); i++){
        func_variables[function_node->get_value(i + 1)] = std::stoi(arguments[i]);
    }

    // Print function variables
    // for(auto &var : func_variables){
    //     std::cout << var.first << " = " << var.second << std::endl;
    // }
    
    //Body of Function
    interpret_stmt_list(function_node->get_child(0), func_variables);

    //std::cout << "Function " << node->get_value() << " returned " << func_variables["return_value"] << std::endl;

    int return_value = func_variables["return_value"];

    return return_value;
}

void interpret(Node* node, std::map<std::string, int> &variables, std::map<std::string, Node*> &functions, bool sif = false){
    if(node->get_type() != NodeType::PROGRAM){
        interpretation_error("Program doesn't start with PROGRAM Node", node);
    }

    functions_defintions = functions;

    // print function names and their arguments
    // for(auto &func : functions_defintions){
    //     std::cout << "Function: " << func.first << std::endl;
    //     for(int i = 1; i < (int)func.second->get_values().size(); i++){
    //         std::cout << "  Argument: " << func.second->get_value(i) << std::endl;
    //     }
    // }

    speculative_exe = sif;
    
    interpret_stmt_list(node->get_child(0), variables);
}

#endif // INTERPRETER_HPP