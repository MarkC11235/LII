#ifndef HTTP_UTILS_HPP
#define HTTP_UTILS_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

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
std::map<int, std::map<std::string, Value>> requests_queue;
std::mutex requests_mutex;

int max_responses = 10;
std::map<int, std::map<std::string, Value>> responses_queue;
std::mutex responses_mutex;

int start_server(int port);
int stop_server();
std::map<std::string, Value> pop_request();
int push_response(int client_fd, std::map<std::string, Value> response);
int send_response(int client_fd);

// Function to split a string by a delimiter
std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }

    tokens.push_back(s.substr(start, end));
    return tokens;
}

std::map<std::string, Value> parse_request(const std::string& request){
    std::map<std::string, Value> parsed_request;

    // split the request into lines
    std::vector<std::string> lines = split(request, "\r\n");

    // parse the first line
    std::vector<std::string> first_line = split(lines[0], " ");
    if(first_line.size() != 3){
        std::cerr << "Invalid request\n";
        return parsed_request;
    }

    parsed_request["method"] = Value{Value_Type::STRING, first_line[0]};
    parsed_request["path"] = Value{Value_Type::STRING, first_line[1]};
    parsed_request["version"] = Value{Value_Type::STRING, first_line[2]};

    // parse the headers
    std::map<std::string, Value> headers;
    for(int i = 1; i < (int)lines.size(); i++){
        std::vector<std::string> header = split(lines[i], ": ");
        if(header.size() != 2){
            std::cerr << "Invalid header\n";
            return parsed_request;
        }

        headers[header[0]] = Value{Value_Type::STRING, header[1]};
    }

    parsed_request["headers"] = Value{Value_Type::STRUCT, headers};

    // parse the body


    return parsed_request;
}

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

        // parse the request
        std::map<std::string, Value> request = parse_request(buffer);

        // push the request to the queue
        {
            std::lock_guard<std::mutex> lock(requests_mutex);
            if(requests_queue.size() >= max_requests){
                std::cerr << "Request queue full\n";
                close(client_fd);
                return;
            }
            requests_queue[client_fd] = request;
        }

        // send a response
        send_response(client_fd);

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
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(sockfd, &readfds);

            struct timeval timeout;
            timeout.tv_sec = 1; // 1 second timeout
            timeout.tv_usec = 0;

            int activity = select(sockfd + 1, &readfds, nullptr, nullptr, &timeout);

            if (activity < 0 && errno != EINTR) {
                std::cerr << "Select error\n";
                break;
            }

            if (activity == 0) {
                // Timeout occurred, continue to check server_running
                continue;
            }

            if (FD_ISSET(sockfd, &readfds)) {
                int client_fd = accept(sockfd, nullptr, nullptr);
                if (client_fd < 0) {
                    if (server_running) {
                        std::cerr << "Failed to accept connection\n";
                    }
                    continue;
                }

                std::thread(handle_client, client_fd).detach();
            }
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
        if (server_thread.joinable()) {
            server_thread.join();
        }

        close(sockfd);

        std::lock_guard<std::mutex> lock(requests_mutex);
        requests_queue.clear();

        std::lock_guard<std::mutex> lock2(responses_mutex);
        responses_queue.clear();

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception in stop_server: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception in stop_server" << std::endl;
        return 1;
    }
}

std::map<std::string, Value> pop_request() {
    std::lock_guard<std::mutex> lock(requests_mutex);
    try {
        if (requests_queue.empty()) {
            return std::map<std::string, Value>();
        }

        std::map<std::string, Value> request = requests_queue.begin()->second;
        int client_fd = requests_queue.begin()->first;
        requests_queue.erase(requests_queue.begin());
        request["client_fd"] = Value{Value_Type::NUMBER, (double)client_fd};

        return request;
    } catch (const std::exception& e) {
        std::cerr << "Exception in get_request: " << e.what() << std::endl;
        return std::map<std::string, Value>();
    } catch (...) {
        std::cerr << "Unknown exception in get_request" << std::endl;
        return std::map<std::string, Value>();
    }
}

int push_response(int client_fd, std::map<std::string, Value> response) {
    try {
        std::lock_guard<std::mutex> lock(responses_mutex);
        if (responses_queue.size() >= max_responses) {
            std::cerr << "Response queue full\n";
            return 1;
        }

        responses_queue[client_fd] = response;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception in push_response: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception in push_response" << std::endl;
        return 1;
    }
}

int send_response(int client_fd) {
    try {
        while(true){
            std::cout << "Checking for response in queue\n";
            {
                std::lock_guard<std::mutex> lock(responses_mutex);
                if(responses_queue.find(client_fd) != responses_queue.end()){
                    std::cout << "Sending response\n";
                    std::map<std::string, Value> response = responses_queue[client_fd];
                    responses_queue.erase(client_fd);

                    std::string response_str = "HTTP/1.1 200 OK\r\n";
                    response_str += "Content-Type: text/html\r\n";
                    response_str += "Content-Length: " + std::to_string(VALUE_AS_STRING(response["body"]).size()) + "\r\n";
                    response_str += "\r\n";
                    response_str += VALUE_AS_STRING(response["body"]);

                    send(client_fd, response_str.c_str(), response_str.size(), 0);
                    close(client_fd);
                    return 0;
                }
            }
            // the lock is released here because the scope of the lock_guard ends
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in send_response: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception in send_response" << std::endl;
        return 1;
    }

    return 1;
}

#endif // HTTP_UTILS_HPP