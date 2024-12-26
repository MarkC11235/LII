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
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>


#include "../Value.hpp"

std::atomic<bool> server_running(true);
std::thread server_thread;
int sockfd;

int max_requests = 10;
std::vector<std::map<std::string, Value>> requests_queue;

void handle_client(int client_fd) {
    try {
        char buffer[256] = {0};
        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            std::cerr << "Error reading from socket\n";
            close(client_fd);
            return;
        }
        else if(bytes_received > 255) {
            std::cerr << "Request too large\n";
            close(client_fd);
            return;
        }

        buffer[bytes_received] = '\0';

        // Process the request (for example, extract the requested file)
        // GET /file.html ...
        // char* f = buffer + 5;
        // *strchr(f, ' ') = 0;

        // For simplicity, we'll just send a basic response
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!";
        send(client_fd, response.c_str(), response.size(), 0);

        close(client_fd);
    } catch (const std::exception& e) {
        std::cerr << "Exception in handle_client: " << e.what() << std::endl;
        close(client_fd);
    } catch (...) {
        std::cerr << "Unknown exception in handle_client" << std::endl;
        close(client_fd);
    }
}

int server_loop(int sockfd) {
    try {
        while (server_running) {
            int client_fd = accept(sockfd, nullptr, nullptr);
            if (client_fd < 0) {
                if (server_running) {
                    std::cerr << "Failed to accept connection\n";
                }
                continue;
            }

            std::thread(handle_client, client_fd).detach();
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in server_loop: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in server_loop" << std::endl;
    }

    close(sockfd);
    return 0;
}

int start_server(int port) {
    try {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Failed to create socket\n";
            return 1;
        }

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "Error binding socket\n";
            close(sockfd);
            return 1;
        }

        if (listen(sockfd, 10) < 0) {
            std::cerr << "Error listening on socket\n";
            close(sockfd);
            return 1;
        }

        server_thread = std::thread(server_loop, sockfd);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception in start_server: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception in start_server" << std::endl;
        return 1;
    }
}

int stop_server() {
    try {
        server_running = false;

        // Create a dummy connection to unblock the accept call
        // TODO: This is a hack and should be replaced with a proper solution
        // The thread sits at accept so it doesn't check the condition in while loop until a connection is made
        int dummy_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (dummy_sock >= 0) {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr("127.0.0.1");
            addr.sin_port = htons(8080); // Use the same port as the server

            connect(dummy_sock, (struct sockaddr*)&addr, sizeof(addr));
            close(dummy_sock);
        }

        if (server_thread.joinable()) {
            server_thread.join();
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception in stop_server: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception in stop_server" << std::endl;
        return 1;
    }
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