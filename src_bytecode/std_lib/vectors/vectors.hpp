#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <string>
#include <vector>
#include <iostream>
#include <algorithm> // std::reverse

#include "../std_lib_helpers/helpers.hpp"

std::vector<Value> vector_create(int size, Value value);
int vector_length(std::vector<Value> a);
std::vector<Value> vector_push(std::vector<Value> a, Value b);
std::vector<Value> vector_pop(std::vector<Value> a);
std::vector<Value> vector_insert(std::vector<Value> a, int index, Value b);
std::vector<Value> vector_remove(std::vector<Value> a, int index);
std::vector<Value> vector_clear(std::vector<Value> a);
Value vector_get(std::vector<Value> a, int index);
std::vector<Value> vector_set(std::vector<Value> a, int index, Value b);
std::vector<Value> vector_slice(std::vector<Value> a, int start, int length);
std::vector<Value> vector_reverse(std::vector<Value> a);
std::vector<Value> vector_join(std::vector<Value> a, std::vector<Value> b);


#endif // VECTOR_HPP