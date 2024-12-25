#ifndef MISC_HPP
#define MISC_HPP

#include <iostream>
#include <cstdlib>

int exit_program(std::string message){
    std::cout << message << std::endl;
    exit(0);
    return 0; // Should never reach here
}

int wait(double seconds){
    clock_t endwait;
    endwait = clock() + seconds * CLOCKS_PER_SEC;
    while (clock() < endwait) {}
    return 0;
}

int system_command(std::string command){
    // TODO: maybe make this return the output of the command
    system(command.c_str());
    return 0;
}

#endif // MISC_HPP