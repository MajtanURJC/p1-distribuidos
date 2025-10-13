#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <signal.h>

#define PORT 8080
#define BUF_SIZE 1024

int end = 0;

void signal_control (int out_signal) {
    end = 1;
}

int main (int argc, char* argv[]) {

    signal(SIGINT, signal_control); 
    setbuf(stdout, NULL);
    char buffer[1024];
    ssize_t size = 0;
    int sockfd;
    int connected;
    int selected;
    
    
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

    connected = connect(sockfd, (struct sockaddr *) &sock, sizeof(sock));
    if (connected < 0) {
        perror("Error on connect");
        exit(1);
    }

    while (1) {
    printf("> ");
    if (fgets(buffer, BUF_SIZE, stdin) == NULL) {
        perror("Error reading stdin");
        break;
    }

    size = send(sockfd, buffer, strlen(buffer), 0);
    if (size < 0) {
        perror("Error on send");
        close(sockfd);
        exit(1);
    }

    if (end == 1) {
        break;
    }

    fd_set readfds;
    struct timeval timeout;

    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 2000000;

    selected = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
    if (selected < 0) {
        perror("select error");
        close(sockfd);
        exit(1);
    }

    if (selected > 0 && FD_ISSET(sockfd, &readfds)) {
        size = recv(sockfd, buffer, BUF_SIZE - 1, 0);
        if (size < 0) {
            perror("Error on receiving");
            close(sockfd);
            exit(1);
        } else if (size == 0) {
            printf("Servidor cerró la conexión.\n");
            break;
        }

        buffer[size] = '\0';
        printf("+++ %s\n", buffer);
    }

    if (end == 1) {
        break;
    }
}

   
    close(sockfd);    
    printf("\nServer stopped with Ctrl+C\n");
    exit(0);   

 }

