#include "misc.hpp"

/*
Prints a message and exits the program
*/
int exit_program(std::string message){
    std::cout << message << std::endl;
    exit(0);
    return 0; // Should never reach here
}

/*
Waits for a number of seconds
*/
int wait(double seconds){
    clock_t endwait;
    endwait = clock() + seconds * CLOCKS_PER_SEC;
    while (clock() < endwait) {}
    return 0;
}

/*
Executes a system command
TODO: maybe make this return the output of the command
*/
int system_command(std::string command){
    system(command.c_str());
    return 0;
}
