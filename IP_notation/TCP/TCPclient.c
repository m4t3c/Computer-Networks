#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char const *argv[])
{
    int sockfd, n;

    if (argc != 3)
    {
        fprintf(stderr, "ERROR: usage is %s hostname port\n", argv[0]);
        exit(1);
    }

    const char *target_ip_str = argv[1];
    uint16_t target_port = atoi(argv[2]);
    struct in_addr addr;
    struct sockaddr_in serv_addr;
    char buffer[512];

    inet_aton(target_ip_str, &addr);

    uint32_t ip_addr = addr.s_addr;
    uint16_t port = htons(target_port);

    char packet[8];
    memcpy(packet, &ip_addr, sizeof(ip_addr));
    packet[4] = '\0';
    memcpy(packet + 5, &port, sizeof(port));
    packet[7] = '\0';


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    } 
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr = addr;
    serv_addr.sin_port = port;

    if (connect(sockfd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
        error("ERROR connecting");
    }
    bzero(buffer, 512);
    memcpy(buffer, packet, sizeof(packet));
    n = write(sockfd, buffer, 8);
    if (n < 0)
    {
        error("ERROR writing to socket");
    }

    return 0;
}
