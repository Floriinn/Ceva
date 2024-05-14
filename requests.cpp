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

char *compute_get_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count)
{
    char *message = (char*) calloc(BUFLEN, sizeof(char));
    char *line =  (char*) calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookies != NULL) {
        for (int i = 0; i < cookies_count; i++) {
            sprintf(line, "Cookie: %s", cookies[i]);
            compute_message(message, line);
        }
    }

    // Step 4: add final new line
    compute_message(message, "");
    return message;
}

std::string compute_post_request(char *host, std::string url, std::string content_type, std::string body_data,
                            char **cookies, int cookies_count)
{
    // char *message = (char*) calloc(BUFLEN, sizeof(char));
    std::string message;
    char *line = (char*) calloc(LINELEN, sizeof(char));
    char *body_data_buffer = (char*) calloc(LINELEN, sizeof(char));
    int content_length = 0;
    // Step 1: write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url.data());
    message = compute_message(message, line);
    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    message = compute_message(message, line);
    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    sprintf(line, "Content-Type: %s", content_type.data());
    message = compute_message(message, line);
    content_length = body_data.length();
    sprintf(line, "Content-Length: %d", content_length);
    message = compute_message(message, line);
    // Step 4 (optional): add cookies
    if (cookies != NULL) {
       for (int i = 0; i < cookies_count; i++) {
            sprintf(line, "Cookie: %s", cookies[i]);
            compute_message(message, line);
        }
    }
    // Step 5: add new line at end of header
    message = compute_message(message, "");
    // Step 6: add the actual payload data
    message.append(body_data);
    cout << message;
    free(line);
    return message;
    
}
