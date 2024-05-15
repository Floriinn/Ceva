#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.hpp"
#include "requests.hpp"
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;

using namespace std;

string compute_get_request(char *host, string url, string query_params,
                            string cookie, int cookies_count, string token)
{
    std::string message;
    char *line = (char*) calloc(LINELEN, sizeof(char));

    if (!query_params.empty()) {
        sprintf(line, "GET %s%s HTTP/1.1", url.data(), query_params.data());
    } else {
        sprintf(line, "GET %s HTTP/1.1", url.data());
    }

    message = compute_message(message, line);

    sprintf(line, "Host: %s", host);
    message = compute_message(message, line);

    if (cookies_count > 0) {
        for (int i = 0; i < cookies_count; i++) {
            sprintf(line, "Cookie: %s", cookie.data());
            message.append(line);
        }
    }
    if (!token.empty()) {
        sprintf(line, "Authorization: Bearer %s", token.data());
        message = compute_message(message, line);
    }

    message = compute_message(message, "");
    
    free(line);
    return message;
}

std::string compute_post_request(char *host, std::string url, std::string content_type, std::string body_data,
                            string cookie, int cookies_count, string token)
{
    std::string message;
    char *line = (char*) calloc(LINELEN, sizeof(char));
    int content_length = 0;
    sprintf(line, "POST %s HTTP/1.1", url.data());
    message = compute_message(message, line);

    sprintf(line, "Host: %s", host);
    message = compute_message(message, line);

    sprintf(line, "Content-Type: %s", content_type.data());
    message = compute_message(message, line);

    content_length = body_data.length();
    sprintf(line, "Content-Length: %d", content_length);
    message = compute_message(message, line);

    if (cookies_count > 0) {
        for (int i = 0; i < cookies_count; i++) {
            sprintf(line, "Cookie: %s", cookie.data());
            message.append(line);
        }
    }
    if (!token.empty()) {
        sprintf(line, "Authorization: Bearer %s", token.data());
        message = compute_message(message, line);
    }
    message = compute_message(message, "");
    message.append(body_data);
    free(line);
    return message;
    
}

string compute_delete_request(char *host, string url, string query_params,
                            string cookie, int cookies_count, string token)
{
    std::string message;
    char *line = (char*) calloc(LINELEN, sizeof(char));

    if (!query_params.empty()) {
        sprintf(line, "DELETE %s%s HTTP/1.1", url.data(), query_params.data());
    }
    message = compute_message(message, line);

    sprintf(line, "Host: %s", host);
    message = compute_message(message, line);

    if (cookies_count > 0) {
        for (int i = 0; i < cookies_count; i++) {
            sprintf(line, "Cookie: %s", cookie.data());
            message.append(line);
        }
    }
    if (!token.empty()) {
        sprintf(line, "Authorization: Bearer %s", token.data());
        message = compute_message(message, line);
    }

    message = compute_message(message, "");
    
    free(line);
    return message;
}