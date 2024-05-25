#include <iostream>
#include <thread>

int main() {
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << "Number of threads = " << n << std::endl;
    return 0;
}