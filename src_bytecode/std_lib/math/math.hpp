#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <random>
#include <vector>
#include <string>

namespace math {

    int random_int(int min, int max);

    double math_sqrt(double value);
    double math_pow(double base, double exponent);

    double abs(double value);

    double ln(double value);
    double log_base_n(double value, double base);

    double sin(double value);
    double cos(double value);
    double tan(double value);

}

#endif // MATH_HPP