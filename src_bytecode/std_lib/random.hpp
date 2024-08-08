#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include <vector>
#include <string>

int random_number(int min, int max){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}


#endif // RANDOM_HPP