#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "http.h"

#define BUF_SIZE 1024
#define MAX_PORT_LEN 6 // maximum character length of a port including null byte
#define HTTP_GET_FMT "GET /%s HTTP/1.0\r\n" \
                     "Host: %s\r\n"         \
                     "User-Agent: getter\r\n\r\n" // format of a HTTP 1.0 request
#define HTTP_HEADER_LEN 47                        // length of http get template including null byte

Buffer *get_http_response(int sockfd);
struct addrinfo *get_server_addrinfo(char *host, int port);

// Make an HTTP query to the given host/page/port
Buffer *http_query(char *host, char *page, int port)
{

    // retrieve server address
    struct addrinfo *server_addr = get_server_addrinfo(host, port);

    // connect to server
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sockfd, server_addr->ai_addr, server_addr->ai_addrlen) == -1)
    {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(server_addr);

    // send GET request
    size_t header_len = HTTP_HEADER_LEN + strlen(host) + strlen(page);
    char *get_req = malloc(header_len);
    sprintf(get_req, HTTP_GET_FMT, page, host);
    if (write(sockfd, get_req, header_len - 1) == -1)
    {
        perror("write");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    free(get_req);

    return get_http_response(sockfd);
}

// Get the addrinfo struct for the given server
struct addrinfo *get_server_addrinfo(char *host, int port)
{
    // initialise server hints
    struct addrinfo server_hints;
    memset(&server_hints, 0, sizeof(server_hints));
    server_hints.ai_family = AF_INET;
    server_hints.ai_socktype = SOCK_STREAM;

    // convert port to string
    char port_str[MAX_PORT_LEN];
    int s = snprintf(port_str, MAX_PORT_LEN, "%d", port);
    if (s < 0 || s > MAX_PORT_LEN)
    {
        perror("port");
        exit(EXIT_FAILURE);
    }

    // get server address
    struct addrinfo *server_addr = NULL;
    s = getaddrinfo(host, port_str, &server_hints, &server_addr);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    return server_addr;
}

// Returns a pointer to an HTTP response buffer
Buffer *get_http_response(int sockfd)
{
    // initialise buffer
    Buffer *ret = malloc(sizeof(Buffer));
    ret->data = malloc(0);
    ret->length = 0;
    int s;

    do
    {
        // write response into buffer
        ret->data = realloc(ret->data, ret->length + BUF_SIZE);
        s = read(sockfd, ret->data + ret->length, BUF_SIZE);
        if (s == -1)
        {
            perror("read");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        ret->length += s;
    } while (s > 0);

    memset(ret->data + ret->length, 0, ret->length % BUF_SIZE); // pad the data with zeros
    return ret;
}

// split http content from the response string
char *http_get_content(Buffer *response)
{

    char *header_end = strstr(response->data, "\r\n\r\n");

    if (header_end)
    {
        return header_end + 4;
    }
    else
    {
        return response->data;
    }
}

Buffer *http_url(const char *url)
{
    char host[BUF_SIZE];
    strncpy(host, url, BUF_SIZE);

    char *page = strstr(host, "/");
    if (page)
    {
        page[0] = '\0';

        ++page;
        return http_query(host, page, 80);
    }
    else
    {

        fprintf(stderr, "could not split url into host/page %s\n", url);
        return NULL;
    }
}
