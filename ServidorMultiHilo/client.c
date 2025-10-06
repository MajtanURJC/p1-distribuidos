#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

#define PORT 8080
#define BUF_SIZE 1024

int end = 0;

int start_receiving (int conn_sock, ssize_t size, char buffer[]) {
    size = recv(conn_sock, buffer, 1024 - 1, 0);  // tamaño fijo del buffer
    if (size < 0 ) 
        return -1;
    buffer[size] = '\0';  
    printf("+++ %s", buffer);
    return 0;
}

int start_sending (int conn_sock, char buffer[], ssize_t size) {
    memset(buffer, 0, 1024);
    printf("> ");
    fgets(buffer, BUF_SIZE, stdin);
    size = send(conn_sock, buffer, strlen(buffer), 0);
    if (size < 0) 
        return -1;
    return 0;
}

void signal_control (int out_signal) {
    end = 1;
}

int main (int argc, char* argv[]) {

    signal(SIGINT, signal_control); 
    char buffer[1024];
    ssize_t size = 0;
    int conv_res;
    int sockfd;
    
    struct sockaddr_in sock;
    sock.sin_family = AF_INET;
    sock.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock.sin_port = htons(PORT);

    sockfd = socket(sock.sin_family, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("Error creating the socket\n");
        exit(1);
    } 

    printf("Socket successfully created...\n");

    if (connect(sockfd, (struct sockaddr *) &sock, sizeof(sock)) < 0) {
        perror("Error on connect");
        exit(1);
    }

    while (end == 0) {

        conv_res = start_sending(sockfd, buffer, size);
        if (conv_res < 0) {
            perror("Error on sending");
            exit(1);
        }

        conv_res = start_receiving(sockfd, size, buffer);
        if (conv_res < 0) {
            perror("Error on receiving");
            exit(1);
        }
    }
   
    close(sockfd);    
    printf("\nServidor detenido con Ctrl+C\n");
    exit(0);   

 }

