#ifndef HTTP_UTILS_HPP
#define HTTP_UTILS_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <thread>
#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


std::atomic<bool> running(true);
std::thread server_thread;

//std::vector<std::map<std::string, VALUE>> requests_queue;

int start_server(int port) {
    

    return 0;
}

int stop_server() {

    return 0;
}

void handle_request(int client_socket) {
    
}

int send_response() {
    return 0;
}

std::string parse_request(const std::string& request) {
    return request;
}

std::string parse_response(const std::string& response) {
    return response;
}


#endif // HTTP_UTILS_HPP