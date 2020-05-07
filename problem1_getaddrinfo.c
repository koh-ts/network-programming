#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

int main(int argc, char **argv){
    unsigned int destport;
    int sock, s, n, cnt = 0;
    char buf[50], deststr[50];
    strcpy (deststr, argv[1]);
    struct addrinfo hints;
    struct addrinfo* res = NULL;
    struct addrinfo* ai;

    if(argc != 3){
        printf("Useage: %s <IP Address> <Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if((destport = (unsigned int) atoi(argv[2])) == 0){
        printf("Invalid destination port number.\n");
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_NUMERICSERV;
    s = getaddrinfo(argv[1], argv[2], &hints, &res);

    if (s != 0) {
        printf("failed to getaddrinfo %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    for(ai = res; ai != NULL; ai = ai->ai_next){
        sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if(sock == -1){
            cnt++;
            if(cnt == 2){
                perror("failed to create a socket.");
                exit(EXIT_FAILURE);
            }
            continue;
        }
        if(connect(sock, ai->ai_addr, ai->ai_addrlen) != -1) break;
        else{
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
    freeaddrinfo(res);
  
}