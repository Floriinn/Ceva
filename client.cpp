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
    
    json post_data;
    json token;
    umap["register"] = 1;
    umap["login"] = 2;
    umap["enter_library"] = 3;
    umap["get_books"] = 4;
    umap["get_book"] = 5;
    umap["add_book"] = 6;
    umap["delete_book"] = 7;
    umap["logout"] = 8;
    umap["exit"] = 9;
    string linie;
    string cookie;
    string username;
    string password;
    int cookie_count = 0;
    string token_string;
    while(true) {
        getline(cin, input_line);
        
        switch (umap[input_line]) {
            case 1:
            {
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

                message = compute_post_request(host, "/api/v1/tema/auth/register", "application/json", post_data.dump(4), cookie, cookie_count, "");
                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);

                if (response.find("201")!=std::string::npos) {
                    cout << "SUCCES! Utilizator înregistrat cu succes!" << endl;
                } else {
                    cout << "EROARE! Numele utilizatorului exista deja!" << endl;
                }
                close_connection(sockfd);
                break;
            }
            case 2:
            {
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
                message = compute_post_request(host, "/api/v1/tema/auth/login", "application/json", post_data.dump(4), cookie, 0, "");
                send_to_server(sockfd, message);
                
                response = receive_from_server(sockfd);
                // Daca in prima parte a raspunsului se gaseste 200, atunci cererea a intors 200 ceea ce inseamna ok
                if (response.substr(0, 256).find("200")!=std::string::npos) {
                    cout << "SUCCESS! Utilizatorul a fost logat cu succes!" << endl;
                } else {
                    
                    cout << "EROARE! Credentialele introduse sunt gresite!" << endl;
                    close_connection(sockfd);
                    break;
                }
                int start;
                int end;
                start = response.find("Cookie:");
                end = response.substr(start,LINELEN).find("Date");
                cookie = response.substr(start + 8, end - 8);
                cookie_count ++;                
                close_connection(sockfd);
                break;
            }
            case 3:
                {
                    // enter_library
                if (cookie_count == 0) {
                    cout << "EROARE, nu esti logat!" << endl;
                    break;
                }
                sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
                message = compute_get_request(host, "/api/v1/tema/library/access", "", cookie, cookie_count, token_string);
                send_to_server(sockfd, message);
                
                response = receive_from_server(sockfd);
                token = json::parse(response.substr(response.find("{\"token"), LINELEN));
                token_string = token["token"];
                cout << "Utilizatorul are acces la biblioteca!" << endl;
                close_connection(sockfd);
                break;
                }
            case 4:
                {
                    // get_books
                if (cookie_count == 0) {
                    cout << "EROARE, nu esti logat!" << endl;
                    break;
                }
                if (token.empty()) {
                    cout << "EROARE, nu ai token!" << endl;
                    break;
                }
                sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
                
                message = compute_get_request(host, "/api/v1/tema/library/books", "", cookie, cookie_count, token_string);
                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);
                
                json nonformat = json::parse(response.substr(response.find("["), 696969));
                cout << nonformat.dump(4);
                close_connection(sockfd);
                break;
                }
            case 5:
                {
                    // get_book
                if (cookie_count == 0) {
                    cout << "EROARE, nu esti logat!" << endl;
                    break;
                }
                if (token.empty()) {
                    cout << "EROARE, nu ai token!" << endl;
                    break;
                }
                sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
                cout << "id=";
                string id;
                getline(cin, id);
                message = compute_get_request(host, "/api/v1/tema/library/books/", id, cookie, cookie_count, token_string);
                send_to_server(sockfd, message);
                
                response = receive_from_server(sockfd);
                
                json nonformat = json::parse(response.substr(response.find("{"), LINELEN));
                cout << nonformat.dump(4) << endl; // Acesta printeaza si erorile in caz ca nu gaseste cartea
                close_connection(sockfd);
                break;
                }
            case 6:
                {
                    // add_book
                if (cookie_count == 0) {
                    cout << "EROARE, nu esti logat!" << endl;
                    break;
                }
                if (token.empty()) {
                    cout << "EROARE, nu ai token!" << endl;
                    break;
                }
                cout << "title=";
                string title;
                getline(cin, title);
                cout << "author=";
                string author;
                getline(cin, author);
                cout << "genre=";
                string genre;
                getline(cin, genre);
                cout << "publisher=";
                string publisher;
                getline(cin, publisher);
                cout << "page_count=";
                string page_count;
                getline(cin, page_count);
                if (!is_number(page_count)) {
                    cout << "EROARE, tip de date incorect pentru numarul de pagini!";
                    break;
                }
                post_data = {
                    {"title", title},
                    {"author", author},
                    {"genre", genre},
                    {"page_count", page_count},
                    {"publisher", publisher},
                    };
                sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
                message = compute_post_request(host, "/api/v1/tema/library/books/", "application/json", post_data.dump(4), cookie, cookie_count, token_string);
                send_to_server(sockfd, message);
                
                response = receive_from_server(sockfd);
                if (response.substr(0, 256).find("200")!=std::string::npos) {
                    cout << "SUCCESS! Cartea a fost adaugata!" << endl;
                } else {
                    cout << "EROARE! Ai gresit tu ceva!" << endl;
                }
                close_connection(sockfd);
                break;
                }
            case 7:
                {
                    // delete_books
                if (cookie_count == 0) {
                    cout << "EROARE, nu esti logat!" << endl;
                    break;
                }
                if (token.empty()) {
                    cout << "EROARE, nu ai token!" << endl;
                    break;
                }
                sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
                cout << "id=";
                string id;
                getline(cin, id);
                message = compute_delete_request(host, "/api/v1/tema/library/books/", id, cookie, cookie_count, token_string);
                send_to_server(sockfd, message);
                
                response = receive_from_server(sockfd);
                if (response.substr(0, 256).find("200")!=std::string::npos) {
                    cout << "Cartea cu id " << id << " a fost stearsa cu succes!";
                } else {
                    cout << "EROARE! Nu a fost gasita cartea la id-ul mentionat!" << endl;
                }
                
                close_connection(sockfd);
                break;
                }
            case 8:
                {
                    // logout
                if (cookie_count == 0) {
                    cout << "EROARE, nu esti logat!" << endl;
                    break;
                }
                sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
                message = compute_get_request(host, "/api/v1/tema/auth/logout", "", cookie, cookie_count, token_string);
                send_to_server(sockfd, message);
                
                response = receive_from_server(sockfd);
                token = "";
                cookie = "";
                cookie_count = 0;
                cout << "Utilizatorul s-a delogat cu succes!" << endl;
                close_connection(sockfd);
                break;
                }
            case 9:
                cout << "Inchidere program";
                break;
            default:
                cout << "lol";
                break;
        }
        if(umap[input_line] == 9) {
            break;
        }
    }

    return 0;
}
