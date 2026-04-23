#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char const *argv[])
{
    int sockfd, portno, n;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[256];

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }

    bzero(buffer, 256);
    clilen = sizeof(cli_addr);
    n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr*) &cli_addr, &clilen);
    if (n < 0)
    {
        error("ERROR reading from socket");
    }
    printf("Here is the message: %s\n", buffer);

    n = sendto(sockfd, "I got your message", 18, 0, (struct sockaddr*) &cli_addr, clilen);
    if (n < 0)
    {
        error("ERROR writing to socket");
    }

    return 0;
}
