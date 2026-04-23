#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char const *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    socklen_t servlen;
    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    printf("Please enter the message: ");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    servlen = sizeof(serv_addr);

    n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &serv_addr, servlen);
    if (n < 0)
    {
        error("ERROR writing on socket");
    }

    bzero(buffer, 256);    
    n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serv_addr, &servlen);
    if (n < 0)
    {
        error("ERROR reading from socket");
    }
    printf("%s\n", buffer);

    return 0;
}
