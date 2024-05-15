#include <string>
#ifndef _REQUESTS_
#define _REQUESTS_
using namespace std;
// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
string compute_get_request(char *host, string url, string query_params,
							string cookie, int cookies_count, string token);

// computes and returns a POST request string (cookies can be NULL if not needed)
std::string compute_post_request(char *host, std::string url, std::string content_type, std::string body_data,
                            string cookie, int cookies_count);

#endif
