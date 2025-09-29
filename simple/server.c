#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define PORT 8080


int start_listening (int sockfd, int start) {
    if(start = 0) {
        int listen_res;
    }
    listen_res = listen(sockfd,1);
    if (listen_res < 0) return -1;
    if(start == 0) {
        printf("Esperando conexión...\n");
        start = 1;
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int new_sock = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen);
        if (new_sock < 0) {
            return -3;
        }
    }
    start_talking()
    return -2;
}

void start_talking ()


int main (int argc, char* argv[]) {
    
    int sockfd;
    int exit_res;
    int bind_res;
    int state;
    int conv_res = 1;
    int start = 0;
    struct sockaddr_in sock;
    sock.sin_family = AF_INET;
    sock.sin_addr.s_addr = INADDR_ANY;
    sock.sin_port = htons(PORT);


    sockfd = socket(sock.sin_family,SOCK_STREAM,0);

    if (sockfd < 0) {
        perror("Error creating the socket\n");
        exit(1);
    } 
    printf("Socket successfully created...\n");

    bind_res = bind(sockfd, (struct sockaddr *) &sock1, sizeof(sock1));
    if (bind_res < 0) {
        perror("Error on bind\n");
        close(sockfd);
        exit(1);
    }

    printf("Server successfully binded...\n")

    conv_res = start_listening(sockfd, start);
    if(conv_res == -1) {
        perror("Listen error\n");
        exit(1);
    } else if (conv_res == -2) {
        perror("Talk error\n");
        exit(1);
    } else if (conv_res == -3) {
        perror("Error on accept\n");
        exit(1);
    }
    

    exit_res = close(sockfd);
    if(exit_res < 1) {
        perror("Exit unssuccessfully\n");
        exit(1);
    }
}