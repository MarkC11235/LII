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
#include <csignal>
#include <netdb.h>


#include "../Value.hpp"

// HTTP SERVER ----------------------------------------------------------------

std::atomic<bool> server_running(true);
std::atomic<bool> sever_should_close(false);
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

/*
To use as a return value to the user when an error occurs
*/
std::map<std::string, Value> make_error_map(const std::string& error) {
    std::map<std::string, Value> error_map;
    error_map["error"] = Value{Value_Type::STRING, error};
    return error_map;
}

/*
Splits a string into a vector of strings based on a delimiter
*/
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

/*
Splits a string into a vector of of two strings based on a delimiter
Chooses the first instance of the delimiter to split on
*/
std::vector<std::string> split_first(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = s.find(delimiter);

    if (pos != std::string::npos) {
        tokens.push_back(s.substr(0, pos));
        tokens.push_back(s.substr(pos + delimiter.length()));
    } else {
        tokens.push_back(s); // No delimiter found, return the whole string
    }

    return tokens;
}

/*
Parses an HTTP request
Returns a map of the request information
TODO: parse the body
*/
std::map<std::string, Value> parse_request(const std::string& request){
    std::map<std::string, Value> parsed_request;

    // split the request into lines
    std::vector<std::string> lines = split(request, "\r\n");

    // parse the first line
    std::vector<std::string> first_line = split(lines[0], " ");
    if(first_line.size() != 3){
        parsed_request.clear();
        parsed_request["error"] = Value{Value_Type::STRING, "Invalid request"};
        return parsed_request;
    }

    parsed_request["method"] = Value{Value_Type::STRING, first_line[0]};
    parsed_request["path"] = Value{Value_Type::STRING, first_line[1]};
    parsed_request["version"] = Value{Value_Type::STRING, first_line[2]};

    // parse the headers
    std::map<std::string, Value> headers;
    for(int i = 1; i < (int)lines.size(); i++){
        std::vector<std::string> header = split_first(lines[i], ": ");
        if(header.size() == 1){
            break; // end of headers
        }
        else if(header.size() != 2){
            std::cout << "Header size: " << header.size() << std::endl;
            std::cout << "Invalid header " << i << ": " << lines[i] << std::endl;
            parsed_request.clear();
            parsed_request["error"] = Value{Value_Type::STRING, "Invalid header"};
            return parsed_request;
        }

        headers[header[0]] = Value{Value_Type::STRING, header[1]};
    }

    parsed_request["headers"] = Value{Value_Type::MAP, headers};

    // parse the body
    std::string body = "";
    for(int i = 1; i < (int)lines.size(); i++){
        if(lines[i].size() == 0){
            body = lines[i+1];
            break;
        }
    }

    parsed_request["body"] = Value{Value_Type::STRING, body};

    return parsed_request;
}

/*
Handles a request from a client
Parses the request, pushes it to the queue, and sends a response back
*/
void handle_client(int client_fd) {
    try {
        constexpr int buffer_size = 1024*1024;
        char buffer[buffer_size] = {0};
        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            std::cerr << "Error reading from socket\n";
            close(client_fd);
            return;
        }
        else if(bytes_received > buffer_size-1) {
            std::cerr << "Request too large\n";
            close(client_fd);
            return;
        }

        buffer[bytes_received] = '\0';

        // parse the request
        std::map<std::string, Value> request = parse_request(buffer);

        if(request.find("error") != request.end() && request.size() == 1){
            std::cerr << VALUE_AS_STRING(request["error"]) << std::endl;
            close(client_fd);
            return;
        }

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

/*
The main server loop that listens for incoming connections
If there is a connection, it creates a new thread to handle the client
After a timeout with no connections, it checks if the server should close (CTRL+C)
*/
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

/*
Checks if (CTRL+C) was pressed and sets the flag saying the server should close
*/
void signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cerr << "Signal received, stopping server...\n";
        sever_should_close = true;
    }
}

/*
Creates a socket, binds it to the port, and starts listening for connections
Creates a new thread to run the server loop
*/
int start_server(int port) {
    try {
        std::signal(SIGINT, signal_handler);
        std::signal(SIGTERM, signal_handler);

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Failed to create socket\n";
            return 1;
        }

        // Set the SO_REUSEADDR option
        // fixes issue with trying to start the server soon after it was stopped
        // There was a problem with binding without this option
        int opt = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            std::cerr << "Failed to set SO_REUSEADDR\n";
            close(sockfd);
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

/*
Returns the flag indicating if the server should close
*/
bool server_should_close() {
    return sever_should_close;
}

/*
Stops the server by setting the server_running flag to false
Joins the server thread to wait for it to finish
Closes the socket
Clears the requests and responses queues
*/
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

/*
Pops a request from the queue in a thread-safe way
Adds the client_fd to the request map
*/
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

/*
Pushes a response to the queue in a thread-safe way
*/
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

/*
Loops until a response is available for the client_fd
The loop sleeps for 1 second between checks
Sends the response and closes the connection
TODO: maybe an issue if the client sends multiple requests before the response is sent
*/
int send_response(int client_fd) {
    try {
        while(true){
            // std::cout << "Checking for response in queue\n";
            {
                std::lock_guard<std::mutex> lock(responses_mutex);
                if(responses_queue.find(client_fd) != responses_queue.end()){
                    // std::cout << "Sending response\n";
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

// --------------------------------------------------------------------------



// HTTP CLIENT ----------------------------------------------------------------

/*
A helper function to build an HTTP request string
Pass in a map with the method, path, headers, and body
Returns a string that can be sent over a socket
*/
std::string build_request(std::map<std::string, Value> request) {
    std::string request_str = VALUE_AS_STRING(request["method"]) + " " + VALUE_AS_STRING(request["path"]) + " HTTP/1.1\r\n";

    if(request.find("headers") == request.end()){
        request["headers"] = Value{Value_Type::MAP, std::map<std::string, Value>()};
    }
    std::map<std::string, Value> headers = VALUE_AS_MAP(request["headers"]);
    for (const auto& [key, value] : headers) {
        request_str += key + ": " + VALUE_AS_STRING(value) + "\r\n";
    }

    request_str += "\r\n";

    if(request.find("body") == request.end()){
        request["body"] = Value{Value_Type::STRING, ""};
    }

    request_str += VALUE_AS_STRING(request["body"]);

    return request_str;
}

/*
Creates a socket, connects to the host and port, sends the request
Receives the response and returns it as a map
*/
std::map<std::string, Value> send_request(std::string host, int port, std::map<std::string, Value> request) {
    try {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Failed to create socket\n";
            return make_error_map("Failed to create socket");
        }

        struct hostent* server = gethostbyname(host.c_str());
        if (server == nullptr) {
            std::cerr << "Failed to get host\n";
            close(sockfd);
            return make_error_map("Failed to get host");
        }

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);

        if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "Failed to connect\n";
            close(sockfd);
            return make_error_map("Failed to connect");
        }

        
        std::string request_str = build_request(request);
        if (send(sockfd, request_str.c_str(), request_str.size(), 0) < 0) {
            std::cerr << "Failed to send request\n";
            close(sockfd);
            return make_error_map("Failed to send request");
        }

        constexpr int buffer_size = 1024*1024;
        char buffer[buffer_size] = {0};
        ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            std::cerr << "Error reading from socket\n";
            close(sockfd);
            return make_error_map("Error reading from socket");
        }
        else if(bytes_received > buffer_size-1) {
            std::cerr << "Response too large\n";
            close(sockfd);
            return make_error_map("Response too large");
        }
 
        buffer[bytes_received] = '\0';

        close(sockfd);

        std::map<std::string, Value> response;
        std::string response_str(buffer);


        std::vector<std::string> lines = split(response_str, "\r\n");
        std::vector<std::string> first_line = split(lines[0], " ");
        if(first_line.size() != 3){
            return make_error_map("Invalid response");
        }

        response["version"] = Value{Value_Type::STRING, first_line[0]};
        response["status_code"] = Value{Value_Type::NUMBER, std::stod(first_line[1])};
        response["status_message"] = Value{Value_Type::STRING, first_line[2]};

        std::map<std::string, Value> headers;
        for(int i = 1; i < (int)lines.size(); i++){
            std::vector<std::string> header = split_first(lines[i], ": ");
            if(header.size() == 1){
                break; // end of headers
            }
            else if(header.size() != 2){
                return make_error_map("Invalid header");
            }

            headers[header[0]] = Value{Value_Type::STRING, header[1]};
        }
        
        response["headers"] = Value{Value_Type::MAP, headers};

        std::string body = "";
        for(int i = 1; i < (int)lines.size(); i++){
            if(lines[i].size() == 0){
                body = lines[i+1];
                break;
            }
        }

        response["body"] = Value{Value_Type::STRING, body};

        return response;


    } catch (const std::exception& e) {
        std::cerr << "Exception in send_request: " << e.what() << std::endl;
        return make_error_map("Exception in send_request");
    } catch (...) {
        std::cerr << "Unknown exception in send_request" << std::endl;
        return make_error_map("Unknown exception in send_request");
    }
}

// ----------------------------------------------------------------------------

#endif // HTTP_UTILS_HPP