/*
  Client TCP Example for Multi
  Sistemas Distribuidos y Concurrentes
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 4) {
        perror("Wrong number of arguments");
        exit(1);
    }

    ssize_t size = -1;
    int socket_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    setbuf(stdout, NULL);

    char *end_ptr;
    long value = strtol(argv[3], &end_ptr, 10);
    
    if (*end_ptr != '\0') {
        fprintf(stderr, "Invalid port: %s\n", argv[3]);
        exit(1);
    }

    unsigned short host_port = (unsigned short) value;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[2]);
    server_addr.sin_port = htons(host_port);

    socket_fd = socket(server_addr.sin_family, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Error creating the socket");
        exit(1);
    }

    if (connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Error on connect");
        exit(1);
    }

    const char *send_msg = "Hello server! From client: ";

    strcpy(buffer, send_msg);
    strcat(buffer, argv[1]);
    strcat(buffer, "\n");

    size = send(socket_fd, buffer, strlen(buffer), 0);
    if (size < 0) {
        perror("Error on send");
        close(socket_fd);
        exit(1);
    }

    size = recv(socket_fd, buffer, BUF_SIZE - 1, 0);
    if (size < 0) {
        perror("Error on receive");
        close(socket_fd);
        exit(1);
    }

    buffer[size] = '\0';
    printf("+++ %s", buffer);

    close(socket_fd);
    exit(0);
}
