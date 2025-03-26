#include "math.hpp"

namespace math {

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

    double abs(double value){
        return std::abs(value);
    }

    double ln(double value){
        return std::log(value);
    }

    double log_base_n(double value, double base){
        return std::log(value) / std::log(base);
    }

    double sin(double value){
        return std::sin(value);
    }

    double cos(double value){
        return std::cos(value);
    }

    double tan(double value){
        return std::tan(value);
    }

}