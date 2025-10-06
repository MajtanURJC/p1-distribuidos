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

void signal_control (int out_signal) {
    end = 1;
}

int main (int argc, char* argv[]) {

    signal(SIGINT, signal_control); 
    char buffer[1024];
    ssize_t size = 0;
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

    while (1) {
            
        printf("> ");
        fgets(buffer, BUF_SIZE, stdin);
        size = send(sockfd,buffer,strlen(buffer),0);
        if (size < 0) {
            perror("Error on send");
            close(sockfd);
            exit(1);
        }

        if(end == 1) {
            break;
        }

        sleep(0.5);
        size = recv(sockfd, buffer, BUF_SIZE - 1, SOCK_NONBLOCK);
        if (size < 0) {
            perror ("Error on receiving");
            close(sockfd);
            exit(1);
        }

        if (size != 0) {
            buffer[size] = '\0';
            printf ("+++ %s", buffer);
            memset(buffer, 0, BUF_SIZE);
        }

        if(end == 1) {
            break;
        }
    }
   
    close(sockfd);    
    printf("\nServidor detenido con Ctrl+C\n");
    exit(0);   

 }

