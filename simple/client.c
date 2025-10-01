#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>


int start_receiving (int conn_sock, ssize_t size, char buffer[]) {

    size = recv (conn_sock,buffer, sizeof(buffer), 0);
    if (size < 0 ) 
        return -1;
    buffer[size] = '\0';  
    printf ("> %s\n", buffer);
    return 0;
}

int start_sending (int conn_sock, char buffer[], ssize_t size) {

    memset (buffer, 0, sizeof(buffer));
    fgets (buffer, sizeof(buffer), stdin);
    size = send (conn_sock,buffer,sizeof(buffer),0);
    if (size < 0) 
        return -1;
    return 0;
}

void signal_control (int out_signal) {

    int exit_res = close (sockfd);
    if (exit_res < 1) {
        perror ("Exit with errors\n");
        exit(1);
    }
    printf("Exited by Ctrl+C\n");
}

int main (int argc, char* argv[]) {

    signal (SIGINT, signal_control); 
    int sockfd;
    int conn_sock;
    char buffer[1024];
    ssize_t size;
    int state;
    int conn_sock;
    int conv_res;
    
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

    conn_sock = connect(sockfd,(struct sockaddr *) &out_sock, sizeof(out_sock), 0);
    if(conn_sock < 0) {
        perror("Error on conect");
        exit(1);
    }

    while (1) {
        conv_res = start_receiving (sockfd, size, buffer);
        if (conv_res < 0) {
            perror ("Error on receiving");
            exit(1);
        }
        conv_res = start_sending (sockfd, buffer, size);
        if (conv_res < 0) {
            perror ("Error on sending");
            exit(1);
        }
    }
}