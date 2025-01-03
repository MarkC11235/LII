#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <string>
#include <vector>
#include <iostream>

#include "helpers.hpp"

/*
Creates a vector of a given size with a given value at each index
*/
std::vector<Value> vector_create(int size, Value value){
    return std::vector<Value>(size, value);
}

/*
Returns the length of the given vector
*/
int vector_length(std::vector<Value> a){
    return a.size();
}

/*
Adds an element to the end of the vector and returns the new vector
*/
std::vector<Value> vector_push(std::vector<Value> a, Value b){
    a.push_back(b);
    return a;
}

/*
Removes the last element from the vector and returns the new vector
*/
std::vector<Value> vector_pop(std::vector<Value> a){
    if(a.size() == 0){
        std_lib_error("vector_pop", "vector is empty");
    }
    a.pop_back();
    return a;
}

/*
Inserts an element into the vector at the given index and returns the new vector
*/
std::vector<Value> vector_insert(std::vector<Value> a, int index, Value b){
    if(index < 0 || index >= (int)a.size()){
        std_lib_error("vector_insert", "index out of bounds");
    }
    a.insert(a.begin() + index, b);
    return a;
}

/*
Removes the element at the given index from the vector and returns the new vector
*/
std::vector<Value> vector_remove(std::vector<Value> a, int index){
    if(index < 0 || index >= (int)a.size()){
        std_lib_error("vector_remove", "index out of bounds");
    }
    a.erase(a.begin() + index);
    return a;
}

/*
Clears the vector and returns the new vector
*/
std::vector<Value> vector_clear(std::vector<Value> a){
    a.clear();
    return a;
}

/*
Returns the element at the given index from the vector
*/
Value vector_get(std::vector<Value> a, int index){
    if(index < 0 || index >= (int)a.size()){
        std_lib_error("vector_get", "index out of bounds");
    }
    return a[index];
}

/*
Sets the element at the given index in the vector to the given value and returns the new vector
*/
std::vector<Value> vector_set(std::vector<Value> a, int index, Value b){
    if(index < 0 || index >= (int)a.size()){
        std_lib_error("vector_set", "index out of bounds");
    }
    a[index] = b;
    return a;
}

/*
Returns a slice of the vector  [start, start + length) inclusive start, exclusive end 
*/
std::vector<Value> vector_slice(std::vector<Value> a, int start, int length){
    if(start < 0 || start >= (int)a.size() || length < 0 || start + length >= (int)a.size()){
        std_lib_error("vector_slice", "index out of bounds");
    }
    return std::vector<Value>(a.begin() + start, a.begin() + start + length);
}

/*
Returns the reverse of the given vector
*/
std::vector<Value> vector_reverse(std::vector<Value> a){
    std::reverse(a.begin(), a.end());
    return a;
}

/*
Concatenates two vectors and returns the new vector
*/
std::vector<Value> vector_join(std::vector<Value> a, std::vector<Value> b){
    a.insert(a.end(), b.begin(), b.end());
    return a;
}


#endif // VECTOR_HPP