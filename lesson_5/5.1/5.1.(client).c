#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int sockfd;
    int n, len;
    char sendline[1000] , recvline[1000];
    struct sockaddr_in servaddr, cliaddr;

    if (argc != 3){
        printf("Usage: %s <IP address>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[2]);

    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    // Заполнение сруктуры адреса сервера

     bzero(&servaddr, sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     servaddr.sin_port = htons(port);
     if (inet_aton(argv[1], &servaddr.sin_addr) == 0) {
        printf("Invalid IP address\n");
        close(sockfd);
        exit(EXIT_FAILURE);
     }

     printf("String => ");
     fgets(sendline, 1000, stdin);

     if (sendto(sockfd, 
                sendline, 
                strlen(sendline)+1, 
                0, 
                (struct sockaddr*) & servaddr, 
                sizeof(servaddr))< 0){
            perror(NULL);
            close(sockfd);
            exit(EXIT_FAILURE);
    }

    if ((n = recvfrom(sockfd,
                recvline, 
                1000, 
                0,
                (struct sockaddr*) NULL ,
                NULL)) < 0){
            perror(NULL);
            close(sockfd);
            exit(EXIT_FAILURE);    
    }
    printf("%s\n", recvline);
    close(sockfd);
    return 0;    
}