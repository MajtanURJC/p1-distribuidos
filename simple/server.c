#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 8080


int main (int argc, char* argv[]) {
    
    int sockfd;
    int res;
    int exit;
    struct sockaddr_in sock1;
    sock.sin_family = AF_INET;
    sock.sin_addr.s_addr = INADDR_ANY;


    sockfd = socket(sock.sin_family,SOCK_STREAM,0);
    if (sockfd < 0) {
        perror("Error creating the socket");
        exit(1);
    } 

    res = bind(sockfd, (struct sockaddr *) &sock1, sizeof(sock1));
    if (res < 0) {
        perror("Error on bind");
        close(sockfd);
        exit(1);
    }

    exit = close(sockdf);
    if(exit < 1) {

    }

}