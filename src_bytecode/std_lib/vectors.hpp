#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <string>
#include <vector>
#include <iostream>

#include "helpers.hpp"

int vector_len(std::vector<Value> a){
    return a.size();
}

std::vector<Value> vector_push(std::vector<Value> a, Value b){
    a.push_back(b);
    return a;
}

std::vector<Value> vector_pop(std::vector<Value> a){
    if(a.size() == 0){
        std_lib_error("vector_pop", "vector is empty");
    }
    a.pop_back();
    return a;
}


#endif // VECTOR_HPP