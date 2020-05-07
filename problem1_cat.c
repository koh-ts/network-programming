#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
    unsigned int destport;
    int sock, s, n, f = 0;
    char buf[50], deststr[50];
    strcpy (deststr, argv[1]);

    if(argc != 3){
        printf("Useage: %s <IP Address> <Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < strlen(argv[1]); ++i){
        if(argv[1][i] == '.'){
            f = 1;
            break;
        }
    }

    if((destport = (unsigned int) atoi(argv[2])) == 0){
        printf("Invalid destination port number.\n");
        exit(EXIT_FAILURE);
    }

    if(f == 1){ //IPv4
        struct sockaddr_in server;
        
        server.sin_family =	AF_INET;
        server.sin_port = htons(destport);

        if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            perror("failed to create a socket.");
            exit(EXIT_FAILURE);
        }

        if(s = (int) inet_pton(AF_INET, deststr, &server.sin_addr) == 0){
            printf("Invalid IP address.\n");
            exit(EXIT_FAILURE);
        };

        if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
            perror("failed to connect.");
            exit(EXIT_FAILURE);
        }
    }else{ // IPv6
        struct sockaddr_in6 server;

        server.sin6_family = AF_INET6;
        server.sin6_port = htons(destport);

        if((sock = socket(AF_INET6, SOCK_STREAM, 0)) < 0){
            perror("failed to create a socket.");
            exit(EXIT_FAILURE);
        }

        if((int) inet_pton(AF_INET6, deststr, &server.sin6_addr) == 0){
            printf("Invalid IP address.\n");
            exit(EXIT_FAILURE);
        };

        if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
            perror("failed to connect.");
            exit(EXIT_FAILURE);
        }
    }

    memset(buf, 0, sizeof(buf));
    if(read(sock, buf, sizeof(buf)) == -1){
        perror("failed to read.");
        exit(EXIT_FAILURE);
    }
    printf("%s", buf);
    close(sock);
    return EXIT_SUCCESS;
}