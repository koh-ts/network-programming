#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h> //strcpy(), memset()
#include <stdlib.h> //exit()
#include <unistd.h> //close()
#include <netdb.h> //getaddrinfo()

int main(int argc, char **argv){
    int sock, s, cnt = 0;
    char buf[50], deststr[50];
    struct addrinfo hints;
    struct addrinfo* res = NULL;
    struct addrinfo* ai;

    if(argc != 3){ //check the num of arguments
        printf("Useage: %s <IP Address> <Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if((atoi(argv[2])) == 0){ //check port number
        printf("Invalid destination port number.\n");
        exit(EXIT_FAILURE);
    }

    //for compatibility with both IPv4 and IPv6
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; //not specify IP version
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_NUMERICSERV;

    if (s = (int) getaddrinfo(argv[1], argv[2], &hints, &res) != 0) {
        printf("failed to getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    //creating socket with either IPv4 or IPv6，and then connect
    for(ai = res; ai != NULL; ai = ai->ai_next){
        sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if(sock == -1){
            cnt++;
            if(cnt == 2){
                perror("failed to create a socket."); //check if socket is created
                exit(EXIT_FAILURE);
            }
            continue;
        }
        if(connect(sock, ai->ai_addr, ai->ai_addrlen) != -1) break;
        else{
            perror("failed to connect."); //check connection
            exit(EXIT_FAILURE);
        }
    }
    freeaddrinfo(res);

    //get data and display it from daytime server
    memset(buf, 0, sizeof(buf));
    if(read(sock, buf, sizeof(buf)) == -1){
        perror("failed to read."); //check if read is success
        exit(EXIT_FAILURE);
    }
    printf("%s", buf);
    close(sock);
}