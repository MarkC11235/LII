#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
// #include "../Value.hpp"

double math_sqrt(double value){
    return std::sqrt(value);
}

double math_pow(double base, double exponent){
    return std::pow(base, exponent);
}

#endif // MATH_HPP