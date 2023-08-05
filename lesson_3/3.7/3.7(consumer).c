#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 256
#define MESSAGE_TYPE 1
#define END_MESSAGE_TYPE 255

struct msg_buffer {
    long mtype;
    char mtext[MAX_MESSAGE_SIZE];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;

    key = ftok(".", 'a');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Полученные сообщения:\n");

    while (1) {
        msgrcv(msgid, &message, sizeof(message.mtext), MESSAGE_TYPE, 0);

        if (message.mtype == END_MESSAGE_TYPE) {
            break;
        }

        printf("%s\n", message.mtext);
    }

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}

