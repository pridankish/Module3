#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MAX_MESSAGE_SIZE 256
#define CLIENT_TO_SERVER_MSG_TYPE 1
#define SERVER_TO_CLIENT_MSG_TYPE 2

int main() {
    mqd_t mq_client_to_server, mq_server_to_client;
    struct mq_attr attr;
    char buffer[MAX_MESSAGE_SIZE + 1];
    ssize_t bytes_read;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;  
    attr.mq_msgsize = MAX_MESSAGE_SIZE;
    attr.mq_curmsgs = 0;

    mq_server_to_client = mq_open("/server_to_client_queue", O_CREAT | O_RDONLY, 0666, &attr);
    if (mq_server_to_client == (mqd_t) -1) {
        perror("mq_open (server to client)");
        exit(EXIT_FAILURE);
    }

    mq_client_to_server = mq_open("/client_to_server_queue", O_CREAT | O_WRONLY, 0666, &attr);
    if (mq_client_to_server == (mqd_t) -1) {
        perror("mq_open (client to server)");
        exit(EXIT_FAILURE);
    }

    printf("Server is working, waiting for messages from the client..\n");

    while (1) {
        bytes_read = mq_receive(mq_client_to_server, buffer, MAX_MESSAGE_SIZE, NULL);
        if (bytes_read == -1) {
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }

        buffer[bytes_read] = '\0';
        printf("Message from client: %s\n", buffer);

        printf("Server message: ");
        fgets(buffer, MAX_MESSAGE_SIZE, stdin);
        buffer[strcspn(buffer, "n")] = '\0';

        mq_send(mq_server_to_client, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    mq_close(mq_client_to_server);
    mq_close(mq_server_to_client);
    mq_unlink("/client_to_server_queue");
    mq_unlink("/server_to_client_queue");

    printf("Server finished.\n");
    return 0;
}
