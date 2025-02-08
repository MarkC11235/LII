#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <string>
#include <vector>
#include <iostream>

#include "helpers.hpp"

std::vector<std::vector<double>> VALUE_AS_MATRIX(std::vector<Value> value){
    std::vector<std::vector<double>> matrix;
    for(auto row : value){
        std::vector<double> matrix_row;
        for(auto element : VALUE_AS_VECTOR(row)){
            matrix_row.push_back(VALUE_AS_NUMBER(element));
        }
        matrix.push_back(matrix_row);
    }
    return matrix;
}

std::vector<Value> matrix_multiply(std::vector<Value> a, std::vector<Value> b){
    std::vector<std::vector<double>> matrix_a = VALUE_AS_MATRIX(a);
    std::vector<std::vector<double>> matrix_b = VALUE_AS_MATRIX(b);

    if(matrix_a[0].size() != matrix_b.size()){
        std::cout << "Matrix A columns: " << matrix_a[0].size() << std::endl;
        std::cout << "Matrix B rows: " << matrix_b.size() << std::endl;
        std_lib_error("matrix_multiply", "Matrix A columns must be equal to Matrix B rows");
    }

    std::vector<std::vector<double>> result;

    for(int i = 0; i < matrix_a.size(); i++){
        std::vector<double> row;
        for(int j = 0; j < matrix_b[0].size(); j++){
            double sum = 0;
            for(int k = 0; k < matrix_a[0].size(); k++){
                sum += matrix_a[i][k] * matrix_b[k][j];
            }
            row.push_back(sum);
        }
        result.push_back(row);
    }

    std::vector<Value> result_value;
    for(auto row : result){
        std::vector<Value> row_value;
        for(auto element : row){
            row_value.push_back({Value_Type::NUMBER, element});
        }
        result_value.push_back({Value_Type::VECTOR, row_value});
    }
    return result_value;
}

#endif // MATRIX_HPP