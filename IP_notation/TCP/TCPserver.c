#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char const *argv[])
{
    const int portno = 1025;
    int sockfd, newsockfd, n;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    uint32_t ip_addr;
    uint16_t port;
    char buffer[512];
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
    {
        error("ERROR on accept");
    }
    
    n = read(newsockfd, buffer, 511);
    if (n < 0)
    {
        error("ERROR reading on socket");
    }
    
    memcpy(&ip_addr, buffer, sizeof(ip_addr));
    memcpy(&port, buffer + 5, sizeof(port));

    struct in_addr addr;
    uint16_t target_port = ntohs(port);
    addr.s_addr = ip_addr;
    char *target_ip_str = inet_ntoa(addr);
    printf("IP is %s; uint32 is %u\n", target_ip_str, ntohl(ip_addr));
    printf("Port is %d; uint16 is %d\n", target_port, target_port);

    return 0;
}
