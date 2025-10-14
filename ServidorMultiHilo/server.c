/*
  Multithreaded TCP Server
  Sistemas Distribuidos y Concurrentes
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

#define BUF_SIZE 1024
#define MAX_CLIENTS 100

int end_flag = 0;
int num_threads = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Function to handle Ctrl+C
void signal_control(int out_signal) {
    end_flag = 1;
}

void *thread_client(void *arg) {
    int conn_sock = *(int *)arg;
    char buffer[BUF_SIZE];
    ssize_t size;

    size = recv(conn_sock, buffer, BUF_SIZE - 1, 0);
    if (size < 0) {
        perror("Error on recv");
    }

    buffer[size] = '\0';
    printf("+++ %s", buffer);

    double min_value = 0.5;
    double max_value = 2.0;
    double numero;

    pthread_mutex_lock(&lock);
    numero = min_value + (rand() / (double)RAND_MAX) * (max_value - min_value);
    pthread_mutex_unlock(&lock);

    usleep((int)(numero * 1000000));

    memset(buffer, 0, BUF_SIZE);
    const char *respuesta = "Hello Client !!!\n";

    size = send(conn_sock, respuesta, strlen(respuesta), 0);
    if (size < 0) {
        perror("Error on send");
    }

    close(conn_sock);
    free(arg);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments\n");
        exit(1);
    }

    struct sockaddr_in sock;
    sock.sin_family = AF_INET;

    signal(SIGINT, signal_control);
    setbuf(stdout, NULL);

    int sockfd;
    int bind_res;
    int listen_res;
    struct sockaddr_in client_addr;
    pthread_t threads[MAX_CLIENTS];

    char *endptr;
    long valor = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid Port: %s\n", argv[1]);
        exit(1);
    }

    unsigned short host_port = (unsigned short)valor;

    sock.sin_addr.s_addr = INADDR_ANY;
    sock.sin_port = htons(host_port);

    sockfd = socket(sock.sin_family, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }
    printf("Socket successfully created...\n");

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
    }

    bind_res = bind(sockfd, (struct sockaddr *)&sock, sizeof(sock));
    if (bind_res < 0) {
        perror("Error on bind");
        close(sockfd);
        exit(1);
    }
    printf("Socket successfully binded...\n");

    listen_res = listen(sockfd, MAX_CLIENTS);
    if (listen_res < 0) {
        perror("Error on listen");
        close(sockfd);
        exit(1);
    }

    printf("Server listening...\n");

    socklen_t addrlen = sizeof(client_addr);

    while (end_flag == 0) {
        if (num_threads >= MAX_CLIENTS) {
            for (int i = 0; i < num_threads; i++) {
                pthread_join(threads[i], NULL);
            }
            num_threads = 0;
        }

        int *conn_sock = malloc(sizeof(int));
        *conn_sock = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen);
        if (*conn_sock < 0) {
            if (end_flag == 1) {
                break;
            }
            perror("Error on accept");
            free(conn_sock);
            continue;
        }

        if (pthread_create(&threads[num_threads], NULL, thread_client,
                           conn_sock) != 0) {
            perror("Error on pthread_create");
            free(conn_sock);
            continue;
        }

        num_threads++;
    }

    close(sockfd);
    printf("\nServer stopped with Ctrl+C\n");
    return 0;
}
