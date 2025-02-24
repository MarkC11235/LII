#ifndef HTTP_UTILS_HPP
#define HTTP_UTILS_HPP

#include <string>
#include <vector>
#include <map>
#include <thread>
#include <atomic>
#include <mutex>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../../helpers/Value.hpp"

// HTTP SERVER declarations
extern std::atomic<bool> server_running;
extern std::atomic<bool> sever_should_close;
extern std::thread server_thread;
extern int sockfd;
extern int max_requests;
extern int max_responses;
extern std::map<int, std::map<std::string, Value>> requests_queue;
extern std::mutex requests_mutex;
extern std::map<int, std::map<std::string, Value>> responses_queue;
extern std::mutex responses_mutex;

// Function declarations
int start_server(int port);
int stop_server();
bool server_should_close();
std::map<std::string, Value> pop_request();
int push_response(int client_fd, std::map<std::string, Value> response);
int send_response(int client_fd);
void handle_client(int client_fd);
int server_loop(int sockfd);
void signal_handler(int signal);

// Helper functions
std::map<std::string, Value> make_error_map(const std::string& error);
std::vector<std::string> split(const std::string& s, const std::string& delimiter);
std::vector<std::string> split_first(const std::string& s, const std::string& delimiter);
std::map<std::string, Value> parse_request(const std::string& request);
std::string build_response(std::map<std::string, Value> response);

// HTTP CLIENT declarations
std::string build_request(std::map<std::string, Value> request);
std::map<std::string, Value> send_request(std::string host, int port, std::map<std::string, Value> request);

#endif // HTTP_UTILS_HPP