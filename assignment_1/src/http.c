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

Buffer *http_query(char *host, char *page, int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

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

    // retrieve server address
    struct addrinfo *server_addr = NULL;
    s = getaddrinfo(host, port_str, &server_hints, &server_addr);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    // connect to server
    if (connect(sockfd, server_addr->ai_addr, server_addr->ai_addrlen) == -1)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
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

int main(int argc, char const *argv[])
{
    return 0;
}
