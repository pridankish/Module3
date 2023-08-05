#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc < 2){
        perror("Port do not provided");
        exit(EXIT_FAILURE);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cliaddr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(EXIT_FAILURE);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 5);

    clilen = sizeof(cliaddr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);

    if (newsockfd < 0) {
        error("Error on accept");
    }

    int num1, num2, answer, choice;

    S: 
    n = write(newsockfd, "Enter number 1: ", strlen("Enter number 1"));
    if (n < 0) error ("Error witing to socket");
    read(newsockfd, &num1, sizeof(int));
    printf("Client - Number 1 is : %d\n", num1);

    n = write(newsockfd, "Enter number 2: ", strlen("Enter number 2"));
    if (n < 0) error ("Error witing to socket");
    read(newsockfd, &num2, sizeof(int));
    printf("Client - Number 2 is : %d\n", num2);

    n = write(newsockfd, "Enter choice: \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n", strlen("Enter choice: \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n"));
    if (n < 0) error ("Error witing to socket");
    read(newsockfd, &choice, sizeof(int));
    printf("Client - Choice is : %d\n", choice);


    switch(choice)
    {
        case 1:
            answer  = num1 + num2;
            break;
        case 2:
            answer  = num1 - num2;
            break;
        case 3:
            answer  = num1 * num2;
            break;
        case 4:
            answer  = num1 / num2;
            break;
        case 5:
            goto Q;
            break;

    }

    write(newsockfd, &answer, sizeof(int));
    if (choice != 5)
        goto S;

    Q:
    close(newsockfd);
    close(sockfd);

    return 0;
}