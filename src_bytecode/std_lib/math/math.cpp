#include "math.hpp"

/*
Generates a random integer [min, max] (inclusive)
*/
int random_int(int min, int max){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

double math_sqrt(double value){
    return std::sqrt(value);
}

double math_pow(double base, double exponent){
    return std::pow(base, exponent);
}
