#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.hpp"
#include "requests.hpp"
#include <unordered_map> 
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;


int main(int argc, char *argv[])
{
    string message;
    string response;
    int sockfd;
    char host[] = "34.246.184.49";
    uint16_t port = 8080;
    string input_line;
    unordered_map<string, int> umap;
    string username;
    string password;
    json post_data;
    umap["register"] = 1;
    umap["login"] = 2;
    int start;
    int end;
    string linie;
    string cookie;
    while(true) {
        getline(cin, input_line);
        
        switch (umap[input_line]) {
            case 1:
                // register
                sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
                cout << "username=";
                getline(cin, username);
                cout << "password=";
                getline(cin, password);
                post_data = {
                    {"username", username},
                    {"password", password},
                    };
                message = compute_post_request(host, "/api/v1/tema/auth/register", "application/json", post_data.dump(4), NULL, 0);
                send_to_server(sockfd, message);
                
                response = receive_from_server(sockfd);
                if (response.find("201")!=std::string::npos) {
                    cout << "SUCCES, te fut.";
                } else {
                    cout << "EROARE, nu te mai fut.";
                }
                // cout << response;
                close_connection(sockfd);
                break;
            case 2:
                // login
                sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
                cout << "username=";
                getline(cin, username);
                cout << "password=";
                getline(cin, password);
                post_data = {
                    {"username", username},
                    {"password", password},
                    };
                message = compute_post_request(host, "/api/v1/tema/auth/login", "application/json", post_data.dump(4), NULL, 0);
                send_to_server(sockfd, message);
                
                response = receive_from_server(sockfd);
                if (response.find("200")!=std::string::npos) {
                    cout << "SUCCES, te fut.\n";
                } else {
                    cout << "EROARE, nu te mai fut.\n";
                }
                start = response.find("Cookie:");
                end = response.substr(start,LINELEN).find("\r\n");
                // cout << response.substr(start, end) << endl << endl;
                // Cookie: = 8
                cookie = response.substr(start + 8, end);
                cout << cookie << endl << endl;
                cout << response;
                close_connection(sockfd);
                break;
            default:
                cout << "lol";
                break;
            }
    }
    // Ex 1.1: GET dummy from main server
    

    // // Ex 1.2: POST dummy and print response from main server
    // sockfd = open_connection("54.170.241.232", 8080, AF_INET, SOCK_STREAM, 0);
    // char *post_data[] = {"field1=value1", "field2=value2", NULL};
    // message = compute_post_request("54.170.241.232", "/api/v1/dummy", "application/x-www-form-urlencoded", post_data, 2, NULL, 0);
    // send_to_server(sockfd, message);
    // response = receive_from_server(sockfd);
    // printf("Ex 1.2 Response:\n%s\n", response);
    // close_connection(sockfd);
    // free(message);
    // free(response);

    // // Ex 2: Login into main server
    // sockfd = open_connection("54.170.241.232", 8080, AF_INET, SOCK_STREAM, 0);
    // char *login_data[] = {"username=student", "password=student"};
    // message = compute_post_request("54.170.241.232", "/api/v1/auth/login", "application/x-www-form-urlencoded", login_data, 2, NULL, 0);
    // send_to_server(sockfd, message);
    // response = receive_from_server(sockfd);
    // printf("Ex 2 Response:\n%s\n", response);
    // close_connection(sockfd);
    // free(message);
    // free(response);

    // // Ex 3: GET weather key from main server
    // sockfd = open_connection("54.170.241.232", 8080, AF_INET, SOCK_STREAM, 0);
    // message = compute_get_request("54.170.241.232", "/api/v1/weather/key", NULL, NULL, 0);
    // send_to_server(sockfd, message);
    // response = receive_from_server(sockfd);
    // printf("Ex 3 Response:\n%s\n", response);
    // close_connection(sockfd);
    // free(message);
    // free(response);

    // // Ex 4: GET weather data from OpenWeather API
    // sockfd = open_connection("api.openweathermap.org", 80, AF_INET, SOCK_STREAM, 0);
    // message = compute_get_request("api.openweathermap.org", "/data/2.5/weather?q=London,uk&appid=your_api_key", NULL, NULL, 0);
    // send_to_server(sockfd, message);
    // response = receive_from_server(sockfd);
    // printf("Ex 4 Response:\n%s\n", response);
    // close_connection(sockfd);
    // free(message);
    // free(response);

    // // Ex 5: POST weather data for verification to main server
    // sockfd = open_connection("54.170.241.232", 8080, AF_INET, SOCK_STREAM, 0);
    // char *weather_data[] = {"temperature=20", "humidity=50", NULL};
    // message = compute_post_request("54.170.241.232", "/verify_weather", "application/x-www-form-urlencoded", weather_data, 2, NULL, 0);
    // send_to_server(sockfd, message);
    // response = receive_from_server(sockfd);
    // printf("Ex 5 Response:\n%s\n", response);
    // close_connection(sockfd);
    // free(message);
    // free(response);

    // // Ex 6: Logout from main server
    // sockfd = open_connection("54.170.241.232", 8080, AF_INET, SOCK_STREAM, 0);
    // message = compute_get_request("54.170.241.232", "/logout", NULL, NULL, 0);
    // send_to_server(sockfd, message);
    // response = receive_from_server(sockfd);
    // printf("Ex 6 Response:\n%s\n", response);
    // close_connection(sockfd);
    // free(message);
    // free(response);

    return 0;
}
