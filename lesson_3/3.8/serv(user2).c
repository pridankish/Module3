#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 256
#define CLIENT_TO_SERVER_MSG_TYPE 1
#define SERVER_TO_CLIENT_MSG_TYPE 2

struct msg_buffer {
    long mtype;
    char mtext[MAX_MESSAGE_SIZE];
};

int main() {
    key_t key;
    int msgid_client_to_server, msgid_server_to_client;
    struct msg_buffer message;

    msgid_server_to_client = msgget(SERVER_TO_CLIENT_MSG_TYPE, 0666);
    if (msgid_server_to_client == -1) {
        perror("msggget (server to client)");
        exit(EXIT_FAILURE);
    }

    msgid_client_to_server = msgget(CLIENT_TO_SERVER_MSG_TYPE, 0666);
    if (msgid_server_to_client == -1) {
        perror("msggget (client to server)");
        exit(EXIT_FAILURE);
    }
    printf("Server is working, waiting for messages from client..\n");

    while (1) {
        msgrcv(msgid_client_to_server, &message, sizeof(message.mtext), CLIENT_TO_SERVER_MSG_TYPE, 0);
        printf("Message from client: %s\n", message.mtext);

        printf("Server message: ");
        fgets(message.mtext, MAX_MESSAGE_SIZE, stdin);
        message.mtext[strcspn(message.mtext, "n")] = '\0';

        message.mtype = SERVER_TO_CLIENT_MSG_TYPE;
        msgsnd(msgid_server_to_client, &message, sizeof(message.mtext), 0);

        if (strcmp(message.mtext, "exit") == 0){
            break;
        }
    }
    printf("Server finish\n");

    return 0;
}