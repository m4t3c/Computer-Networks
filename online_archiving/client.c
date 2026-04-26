#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define CHUNK_SIZE 4096

void error(char *msg){
    perror(msg);
    exit(1);
}

void send_file_to_socket(int sockfd, char *filename) {
    int n;
    struct stat st;
    if (stat(filename, &st) < 0)
    {
        fprintf(stderr, "Could not stat file %s\n", filename);
        return;
    }
    
    char header[60];
    memset(header, ' ', 60);

    char buff_name[17];
    snprintf(buff_name, 17, "%-15s/", filename);
    memcpy(header + 0,  buff_name,    16);
    memcpy(header + 16, "0           ", 12);
    memcpy(header + 28, "0     ",       6);
    memcpy(header + 34, "0     ",       6);
    memcpy(header + 40, "644     ",     8);

    char buff_size[11];
    snprintf(buff_size, 11, "%-10ld", (long)st.st_size);
    memcpy(header + 48, buff_size, 10);

    memcpy(header + 58, "`\n", 2);

    n = write(sockfd, header, 60);
    if (n < 0)
    {
        fprintf(stderr, "ERROR writing on socket\n");
        return;
    }
    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", filename);
        return;
    }

    char buffer[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, CHUNK_SIZE, fp)) > 0)
    {
        n = write(sockfd, buffer, bytes_read);
        if (n < 0)
        {
            fprintf(stderr, "ERROR writing to socket\n");
            return;
        }
    }
    fclose(fp);

    if (st.st_size % 2 != 0)
    {
        n = write(sockfd, "\n", 1);
        if (n < 0)
        {
            fprintf(stderr, "ERROR writing to socket\n");
            return;
        }
        
    }
    
}

int main(int argc, char *argv[]){
    
    int sockfd, n, portno;
    struct sockaddr_in serv_addr;

    if (argc < 4)
    {
        fprintf(stderr, "usage is: %s hostname port <file1> [<file2> ...]", argv[0]);
        exit(1);
    }
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    inet_aton(argv[1], &serv_addr.sin_addr);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }
    
    n = write(sockfd, "!<arch>\n", 8);
    if (n < 0)
    {
        error("ERROR writing to socket");
    }
    for (size_t i = 3; i < argc; i++)
    {
        send_file_to_socket(sockfd, argv[i]);
    }
    close(sockfd);        

    return 0;
}
