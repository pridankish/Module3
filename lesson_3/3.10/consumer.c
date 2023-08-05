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

    mq = mq_open("/my_queue", O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    mq_getattr(mq, &attr);

    printf("Полученные сообщения:\n");

    while (1) {
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_MESSAGE_SIZE, NULL);
        if (bytes_read == -1) {
            perror("mq_receive");
            break;
        }

        // Если получено сообщение с типом END_MESSAGE_TYPE (255), завершаем чтение
        if (buffer[0] == END_MESSAGE_TYPE && bytes_read == 1)
            break;

        buffer[bytes_read] = '\0';
        printf("%s\n", buffer);
    }

    mq_close(mq);

    return 0;
}
