#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/ipc.h>


#define MAX_MESSAGE_SIZE 256
#define CLIENT_TO_SERVER_MSG_TYPE 1
#define SERVER_TO_CLIENT_MSG_TYPE 2

struct msg_buffer {
    long mtype;
    char mtext[MAX_MESSAGE_SIZE];
};


int main(){
    key_t key;
    int msgid_client_to_server, msgid_server_to_client;
    struct msg_buffer message;

    msgid_client_to_server = msgget(CLIENT_TO_SERVER_MSG_TYPE, 0666 | IPC_CREAT);
    if (msgid_client_to_server == -1){
        perror("msget (clien to server)");
        exit(EXIT_FAILURE);
    }

    msgid_server_to_client = msgget(SERVER_TO_CLIENT_MSG_TYPE, 0666 | IPC_CREAT);
    if (msgid_client_to_server == -1){
        perror("msget (clien to server)");
        exit(EXIT_FAILURE);
    }

    printf("Enter strings to send to server ('exit' for quit):\n");

    while(1) {
        printf("Client message: ");
        fgets(message.mtext, MAX_MESSAGE_SIZE, stdin);
        message.mtext[strcspn(message.mtext, "n")] = '\0';

        message.mtype = CLIENT_TO_SERVER_MSG_TYPE;
        msgsnd(msgid_client_to_server, &message, sizeof(message.mtext), 0);

        if (strcmp(message.mtext, "exit") == 0) {
            break;
        }
        msgrcv(msgid_server_to_client, &message, sizeof(message.mtext), SERVER_TO_CLIENT_MSG_TYPE, 0);

        printf("Answer from server: %s\n", message.mtext);
    }
    printf("Client finish\n");

    msgctl(msgid_client_to_server, IPC_RMID, NULL);
    msgctl(msgid_server_to_client, IPC_RMID, NULL);

    return 0;
}