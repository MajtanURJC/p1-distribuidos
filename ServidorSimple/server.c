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

    signal (SIGINT, signal_control);
    int sockfd;
    int bind_res;
    int listen_res;
    int conn_sock;
    char buffer[BUF_SIZE];
    ssize_t size = -1;
    
    struct sockaddr_in sock;
    sock.sin_family = AF_INET;
    sock.sin_addr.s_addr = INADDR_ANY;
    sock.sin_port = htons(PORT);

    struct sockaddr out_sock;

    sockfd = socket (sock.sin_family,SOCK_STREAM,0);

    if (sockfd < 0) {
        perror ("Error creating the socket\n");
        exit(1);
    } 
    printf ("Socket successfully created...\n");

    bind_res = bind(sockfd, (struct sockaddr *) &sock, sizeof(sock));
    if (bind_res < 0) {
        perror("Error on bind\n");
        close(sockfd);
        exit(1);
    }

    printf("Server successfully binded...\n");

    listen_res = listen (sockfd,1);
    if (listen_res < 0) {
        perror ("Error on listen");
        exit(1);
    }
    printf("Server listening...\n");

    socklen_t addrlen = sizeof(out_sock);
    conn_sock = accept(sockfd, (struct sockaddr *) &out_sock, &addrlen);
    close(sockfd);
    if(conn_sock < 0){
        perror ("Error on accept");
        exit(1);
    }

    while (1) {
        size = recv(conn_sock, buffer, BUF_SIZE - 1, 0);
        if (size < 0) {
            perror ("Error on receiving");
            close(conn_sock);
            exit(1);
        }
        buffer[size] = '\0';  
        printf ("+++ %s", buffer);
        
        if(end == 1) {
            break;
        }

        memset(buffer, 0, BUF_SIZE);
        printf("> ");
        fgets(buffer, BUF_SIZE, stdin);
        size = send(conn_sock,buffer,strlen(buffer),0);
        if (size < 0) {
            perror("Error on send");
            close(conn_sock);
            exit(1);
        }
          
        if(end == 1) {
            break;
        }
    }

    close(conn_sock);    
    printf("\nServidor detenido con Ctrl+C\n");
    exit(0);

}