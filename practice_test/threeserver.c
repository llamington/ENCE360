#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#define MAXDATASIZE 1024

int main(int argc, char *argv[])
{

	if (argc != 2)
	{
		fprintf(stderr, "usage: threeServer port-number\n");
		exit(1);
	}

	// place your code between the lines of //
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////

	struct sockaddr_in sa, caller;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(atoi(argv[1]));

	// place your code between the lines of //
	///////////////////////////////////////////////////////

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//////////////////////////////////////////////////////

	// place your code between the lines of //
	///////////////////////////////////////////////////////
	bind(sockfd, (struct sockaddr *)&sa, sizeof(sa));
	listen(sockfd, 1);
	///////////////////////////////////////////////////////

	socklen_t length = sizeof(caller);
	// place your code between the lines of //
	/////////////////////////////////////////////////////////////
	int cfd = accept(sockfd, (struct sockaddr *)&caller, &length);
	/////////////////////////////////////////////////////////////

	char message[MAXDATASIZE] = "congrats you successfully connected to the server!";
	while (strlen(message) > 0)
	{
		int numbytes; // number of bytes of data read from socket

		// send data to the client and then get data back from the client:
		// place your code between the lines of //
		///////////////////////////////////////////////////////
		write(cfd, &message, strlen(message));
		numbytes = read(cfd, &message, MAXDATASIZE - 1);
		///////////////////////////////////////////////////////

		message[numbytes - 1] = '\0';
	}

	// place your code between the lines of //
	///////////////////////////////////////////////////////
	close(cfd);
	close(sockfd);
	///////////////////////////////////////////////////////

	exit(0);
}
