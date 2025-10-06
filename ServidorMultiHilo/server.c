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

    size = recv(conn_sock, buffer, BUF_SIZE - 1, 0);
    if (size < 0 ) 
        return -1;
    buffer[size] = '\0';  
    printf ("+++ %s", buffer);
    return 0;
}

int start_sending (int conn_sock, char buffer[], ssize_t size) {
    
    memset(buffer, 0, BUF_SIZE);
    printf("> ");
    fgets(buffer, BUF_SIZE, stdin);
    size = send (conn_sock,buffer,strlen(buffer),0);
    if (size < 0) 
        return -1;
    return 0;
}

void signal_control (int out_signal) {
    end = 1;
}

int main (int argc, char* argv[]) {

    argc ++;
    argv --;

    struct sockaddr_in sock;
    sock.sin_family = AF_INET;

    if (argc == 1) {
        int n_clients = argv[0]; 
    } else if (argc == 2 ) {

    } else {
        sock.sin_addr.s_addr = argv[0];
        sock.sin_port = argv[1];
    }

    signal (SIGINT, signal_control);
    int sockfd;
    int bind_res;
    int listen_res;
    int conv_res;
    int conn_sock;
    char buffer[BUF_SIZE];
    ssize_t size = 0;
    
    
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

    while (end == 0) {
        conv_res = start_receiving (conn_sock, size, buffer);
        if (conv_res < 0) {
            perror ("Error on receiving");
            exit(1);
        }

        conv_res = start_sending (conn_sock, buffer,size);
        if (conv_res < 0) {
            perror ("Error on sending");
            exit(1);
        }          
    }

    close(conn_sock);    
    printf("\nServidor detenido con Ctrl+C\n");
    exit(0);

}