#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// struct sockaddr_in
// {
//     __SOCKADDR_COMMON (sin_);
//     in_port_t sin_port; /* Port number.*/
//     struct in_addr sin_addr; /* Internet address.*/
//     /* Pad to size of 'struct sockaddr'. */
//     unsigned char sin_zero[sizeof (struct sockaddr) - __SOCKADDR_COMMON_SIZE - sizeof (in_port_t) - sizeof (struct in_addr)];
// };
// struct sockaddr_in6
// {
//     __SOCKADDR_COMMON (sin6_);
//     in_port_t sin6_port; /* Transport layer port #	*/
//     uint32_t sin6_flowinfo; /* IPv6 flow information */
//     struct in6_addr	sin6_addr; /* IPv6 address */
//     uint32_t sin6_scope_id; /* IPv6 scope-id */
// };

// #define	__SOCKADDR_COMMON(sa_prefix) sa_family_t sa_prefix##family

int main(int argc, char **argv){
    unsigned int destport;
    // struct sockaddr_in server;
    int sock, s, n, f = 0;
    char buf[30], deststr[30];
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

    if(f == 1){
        struct sockaddr_in server;

        if((destport = (unsigned int) atoi(argv[2])) == 0){
            printf("Invalid destination port number.\n");
            exit(EXIT_FAILURE);
        }
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

        memset(buf, 0, sizeof(buf));
        n =	read(sock, buf, sizeof(buf));
        printf("%s", buf);

        close(sock);
        return EXIT_SUCCESS;

    }else{
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

        memset(buf, 0, sizeof(buf));
        n =	read(sock, buf, sizeof(buf));
        printf("%s", buf);

        close(sock);
        return EXIT_SUCCESS;
    }
}