#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SIZE 5

void sem_wait(int semid, int sem_num) {
    struct sembuf op;
    op.sem_num = sem_num;
    op.sem_op = -1;
    op.sem_flg = 0;
    semop(semid, &op, 1);
}

void sem_post(int semid, int sem_num) {
    struct sembuf op;
    op.sem_num = sem_num;
    op.sem_op = 1;
    op.sem_flg = 0;
    semop(semid, &op, 1);
}

int main(int argc, char* argv[]) {
    int arr[SIZE];
    srand(time(NULL));
    int i;
    for (i = 0; i < SIZE; i++) {
        arr[i] = rand() % 100;
        printf("Generated %d\n", arr[i]);
    }

    int fd = open("sum", O_WRONLY);
    if (fd == -1) {
        return 1;
    }

    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        return 1;
    }

    semctl(semid, 0, SETVAL, 1);

    sem_wait(semid, 0); // Захватываем семафор для записи в файл

    if (write(fd, arr, sizeof(int) * SIZE) == -1) {
        sem_post(semid, 0); // Освобождаем семафор при ошибке
        return 2;
    }

    sem_post(semid, 0); // Освобождаем семафор после успешной записи
    close(fd);
    semctl(semid, 0, IPC_RMID); // Удаляем семафор
    return 0;
}
