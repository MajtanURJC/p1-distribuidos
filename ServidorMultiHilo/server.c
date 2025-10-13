#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define MAX_CLIENTES 100

int end = 0;
int num_threads = 0;

// Función para manejar Ctrl+C
void signal_control(int out_signal) {
    end = 1;
}

void* thread_client(void* arg) {
    int conn_sock = *(int*)arg;
    char buffer[BUF_SIZE];
    ssize_t size;

    

    size = recv(conn_sock, buffer, BUF_SIZE - 1, 0);
    if (size < 0) {
        perror("Error on recv");
    }
    buffer[size] = '\0';
    printf("+++ %s", buffer);


    memset(buffer, 0, BUF_SIZE);
    const char* respuesta = "Hello Client !!!\n";
    size = send(conn_sock, respuesta, strlen(respuesta), 0);
    if (size < 0) {
        perror("Error on send");
    }

    close(conn_sock);
    num_threads--;
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        perror("Necesita un argumento");
        exit(1);
    }

    struct sockaddr_in sock;
    sock.sin_family = AF_INET;

    signal(SIGINT, signal_control);

    int sockfd;
    int bind_res;
    int listen_res;
    struct sockaddr_in client_addr;
    pthread_t threads[MAX_CLIENTES];
    

    // Convertir el argumento a unsigned uint
    char *endptr;
    long valor = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid Port: %s\n", argv[1]);
        exit(1);
    }

    unsigned short puerto_host = (unsigned short) valor;

    sock.sin_addr.s_addr = INADDR_ANY;
    sock.sin_port = htons(puerto_host);


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


    bind_res = bind(sockfd, (struct sockaddr*)&sock, sizeof(sock));
    if (bind_res < 0) {
        perror("Error en bind");
        close(sockfd);
        exit(1);
    }
    printf("Socket successfully binded...\n");


    listen_res = listen(sockfd, MAX_CLIENTES);
    if (listen_res < 0) {
        perror("Error en listen");
        close(sockfd);
        exit(1);
    }

    printf("Server listening...\n");

    

    socklen_t addrlen = sizeof(client_addr);


    while (end == 0) {
        if (num_threads >= MAX_CLIENTES) {
            break;
        }

        int *conn_sock = malloc(sizeof(int));
        *conn_sock = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
        if (*conn_sock < 0) {
            if(end == 1) {
                break;
            }
            perror("Error on accept");
            free(conn_sock);
            continue;
        }

        if (pthread_create(&threads[num_threads], NULL, thread_client, conn_sock) != 0) {
            perror("Error on pthread");
            free(conn_sock);
            continue;
        }
        num_threads++;

    }
    

    close(sockfd);

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nServidor detenido con Ctrl+C\n");
    return 0;
}
