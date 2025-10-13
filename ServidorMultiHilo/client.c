#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

#define BUF_SIZE 1024

int main (int argc, char* argv[]) {

    if(argc != 4) {
        perror("Wrong number of arguments");
        exit(1);
    }

    ssize_t size = -1;
    int sockfd;
    struct sockaddr_in sock;
    char buffer[BUF_SIZE];
    setbuf(stdout, NULL);


    char *endptr;
    long valor = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid port: %s\n", argv[3]);
        exit(1);
    }

    unsigned short host_port = (unsigned short) valor;

    sock.sin_family = AF_INET;
    sock.sin_addr.s_addr = inet_addr(argv[2]);  
    sock.sin_port = htons(host_port);

    sockfd = socket(sock.sin_family, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("Error creating the socket\n");
        exit(1);
    } 


    if (connect(sockfd, (struct sockaddr *) &sock, sizeof(sock)) < 0) {
        perror("Error on connect");
        exit(1);
    }

    

    const char *envio = "Hello server! From client: ";

    strcpy(buffer, envio);
    strcat(buffer, argv[1]);
    strcat(buffer, "\n");

    size = send(sockfd, buffer, strlen(buffer), 0);
    if (size < 0) {
        perror("Error on send");
        close(sockfd);
        exit(1);
    }

    size = recv(sockfd, buffer, BUF_SIZE - 1, 0);
    if (size < 0) {
        perror ("Error on receiving");
        close(sockfd);
        exit(1);
    }
    buffer[size] = '\0';  
    printf ("+++ %s", buffer);
   
    close(sockfd);    
    exit(0);   
}
