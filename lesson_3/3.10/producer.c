#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MAX_MESSAGE_SIZE 256
#define END_MESSAGE_TYPE 255

int main() {
    mqd_t mq;
    char buffer[MAX_MESSAGE_SIZE];
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MESSAGE_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open("/my_queue", O_CREAT | O_WRONLY, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    printf("Введите строки для отправки в очередь (введите 'exit' для завершения):\n");

    while (1) {
        fgets(buffer, MAX_MESSAGE_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            mq_send(mq, buffer, strlen(buffer), END_MESSAGE_TYPE);
            break;
        }

        mq_send(mq, buffer, strlen(buffer), 1);
    }

    printf("Отправка завершена.\n");

    mq_close(mq);
    mq_unlink("/my_queue");

    return 0;
}
