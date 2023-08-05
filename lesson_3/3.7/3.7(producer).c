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
        exit(1);
    }

    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Введите строки для отправки в очередь (введите 'exit' для завершения):\n");

    while (1) {
        fgets(message.mtext, MAX_MESSAGE_SIZE, stdin);

        message.mtext[strcspn(message.mtext, "\n")] = '\0';

        if (strcmp(message.mtext, "exit") == 0) {
            message.mtype = END_MESSAGE_TYPE;
            msgsnd(msgid, &message, sizeof(message.mtext), 0);
            break;
        }

        message.mtype = MESSAGE_TYPE;
        msgsnd(msgid, &message, sizeof(message.mtext), 0);
    }

    printf("Отправка завершена.\n");

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
