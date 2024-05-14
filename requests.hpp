#include <string>
#ifndef _REQUESTS_
#define _REQUESTS_

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *compute_get_request(char *host, char *url, char *query_params,
							char **cookies, int cookies_count);

// computes and returns a POST request string (cookies can be NULL if not needed)
std::string compute_post_request(char *host, std::string url, std::string content_type, std::string body_data,
                            char **cookies, int cookies_count);

#endif
