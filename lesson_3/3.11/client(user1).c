#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MAX_MESSAGE_SIZE 256
#define CLIENT_TO_SERVER_MSG_TYPE 1
#define SERVER_TO_CLIENT_MSG_TYPE 2

struct msg_buffer {
    long mtype;
    char mtext[MAX_MESSAGE_SIZE];
};

int main() {
    mqd_t mq_client_to_server, mq_server_to_client;
    struct msg_buffer message;
    char buffer[MAX_MESSAGE_SIZE + 1];

    mq_client_to_server = mq_open("/client_to_server_queue", O_WRONLY | O_CREAT, 0666, NULL);
    if (mq_client_to_server == (mqd_t)-1) {
        perror("mq_open (client to server)");
        exit(EXIT_FAILURE);
    }

    mq_server_to_client = mq_open("/server_to_client_queue", O_RDONLY | O_CREAT, 0666, NULL);
    if (mq_server_to_client == (mqd_t)-1) {
        perror("mq_open (server to client)");
        exit(EXIT_FAILURE);
    }

    printf("Enter strings to send to server ('exit' to quit):\n");

    while (1) {
        printf("Client message: ");
        fgets(buffer, MAX_MESSAGE_SIZE, stdin);
        buffer[strcspn(buffer, "n")] = '\0';

        message.mtype = CLIENT_TO_SERVER_MSG_TYPE;
        strncpy(message.mtext, buffer, MAX_MESSAGE_SIZE);
        mq_send(mq_client_to_server, (const char*)&message, sizeof(message.mtext), 0);

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        ssize_t bytes_received = mq_receive(mq_server_to_client, (char*)&message, sizeof(message.mtext), NULL);
        if (bytes_received == -1) {
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }
        message.mtext[bytes_received] = '\0';

        printf("Answer from server: %s\n", message.mtext);
    }

    mq_close(mq_client_to_server);
    mq_close(mq_server_to_client);
    mq_unlink("/client_to_server_queue");
    mq_unlink("/server_to_client_queue");

    printf("Client finish\n");

    return 0;
}
