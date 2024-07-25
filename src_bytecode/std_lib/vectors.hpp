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

std::vector<Value> vector_insert(std::vector<Value> a, int index, Value b){
    if(index < 0 || index >= (int)a.size()){
        std_lib_error("vector_insert", "index out of bounds");
    }
    a.insert(a.begin() + index, b);
    return a;
}

std::vector<Value> vector_remove(std::vector<Value> a, int index){
    if(index < 0 || index >= (int)a.size()){
        std_lib_error("vector_remove", "index out of bounds");
    }
    a.erase(a.begin() + index);
    return a;
}

std::vector<Value> vector_clear(std::vector<Value> a){
    a.clear();
    return a;
}

Value vector_get(std::vector<Value> a, int index){
    if(index < 0 || index >= (int)a.size()){
        std_lib_error("vector_get", "index out of bounds");
    }
    return a[index];
}

std::vector<Value> vector_set(std::vector<Value> a, int index, Value b){
    if(index < 0 || index >= (int)a.size()){
        std_lib_error("vector_set", "index out of bounds");
    }
    a[index] = b;
    return a;
}

std::vector<Value> vector_slice(std::vector<Value> a, int start, int end){
    if(start < 0 || start >= (int)a.size() || end < 0 || end >= (int)a.size()){
        std_lib_error("vector_slice", "index out of bounds");
    }
    return std::vector<Value>(a.begin() + start, a.begin() + end);
}

std::vector<Value> vector_reverse(std::vector<Value> a){
    std::reverse(a.begin(), a.end());
    return a;
}

std::vector<Value> vector_concat(std::vector<Value> a, std::vector<Value> b){
    a.insert(a.end(), b.begin(), b.end());
    return a;
}


#endif // VECTOR_HPP