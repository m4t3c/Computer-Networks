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
    int portno, sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    int n;

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }
    
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        error("ERROR host not provided");
    
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }
    
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
    {
        error("ERROR reading from socket");
    }
    
    char msg[256] = "Server name is: ";
    strcpy(msg + 16, buffer + 13);
    printf("%s\n", msg);

    return 0;
}
