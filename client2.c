#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h> //strcpy(), memset()
#include <stdlib.h> //exit()
#include <unistd.h> //close()

int main(int argc, char **argv){
    unsigned int destport;
    int sock, f = 0;
    char buf[50], deststr[50];
    
    if(argc != 3){ //check the num of arguments
        printf("Useage: %s <IP Address> <Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    strcpy (deststr, argv[1]);
    for(int i = 0; i < strlen(argv[1]); ++i){ //check whether IPv4 or IPv6
        if(argv[1][i] == '.'){
            f = 1;
            break;
        }
    }

    if((destport = (unsigned int) atoi(argv[2])) == 0){ //check port number
        printf("Invalid destination port number.\n");
        exit(EXIT_FAILURE);
    }

    if(f == 1){ //IPv4
        struct sockaddr_in server;
        
        server.sin_family =	AF_INET;
        server.sin_port = htons(destport);

        if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //check if socket is created
            perror("failed to create a socket.");
            exit(EXIT_FAILURE);
        }

        if(inet_pton(AF_INET, deststr, &server.sin_addr) == 0){ //check if IP address is valid
            printf("Invalid IP address.\n");
            exit(EXIT_FAILURE);
        };

        if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0){ //check connection
            perror("failed to connect.");
            exit(EXIT_FAILURE);
        }
    }else{ // IPv6
        struct sockaddr_in6 server;

        server.sin6_family = AF_INET6;
        server.sin6_port = htons(destport);

        if((sock = socket(AF_INET6, SOCK_STREAM, 0)) < 0){ //check if socket is created
            perror("failed to create a socket.");
            exit(EXIT_FAILURE);
        }

        if(inet_pton(AF_INET6, deststr, &server.sin6_addr) == 0){ //check if IP address is valid
            printf("Invalid IP address.\n");
            exit(EXIT_FAILURE);
        };

        if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0){ //check connection
            perror("failed to connect.");
            exit(EXIT_FAILURE);
        }
    }

    //get data and display it from daytime server
    memset(buf, 0, sizeof(buf));
    if(read(sock, buf, sizeof(buf)) == -1){
        perror("failed to read."); //check if read is success
        exit(EXIT_FAILURE);
    }
    printf("%s", buf);
    close(sock);
    return EXIT_SUCCESS;
}