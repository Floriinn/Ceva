#include <string>
#ifndef _HELPERS_
#define _HELPERS_

#define BUFLEN 4096
#define LINELEN 1000

// shows the current error
void error(const char *msg);

// adds a line to a string message
std::string compute_message(std::string message, std::string line);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(std::string host_ip, uint16_t portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, std::string message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
char *basic_extract_json_response(char *str);

#endif
