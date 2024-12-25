#ifndef MISC_HPP
#define MISC_HPP

#include <iostream>
#include <cstdlib>

void exit_program(std::string message){
    std::cout << message << std::endl;
    exit(0);
}

void wait(double seconds){
    clock_t endwait;
    endwait = clock() + seconds * CLOCKS_PER_SEC;
    while (clock() < endwait) {}
}

void system_command(std::string command){
    system(command.c_str());
}

#endif // MISC_HPP