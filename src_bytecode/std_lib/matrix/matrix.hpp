#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <string>
#include <vector>
#include <iostream>

#include "../std_lib_helpers/helpers.hpp"

std::vector<std::vector<double>> VALUE_AS_MATRIX(std::vector<Value> value);

std::vector<Value> matrix_multiply(std::vector<Value> a, std::vector<Value> b);

#endif // MATRIX_HPP