#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
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
    int fd = open("sum", O_RDONLY);
    if (fd == -1) {
        return 1;
    }

    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        return 1;
    }

    semctl(semid, 0, SETVAL, 1);

    sem_wait(semid, 0); // Захватываем семафор для чтения из файла

    int i;
    for(i = 0; i < SIZE; i++) {
        if (read(fd, &arr[i], sizeof(int)) == -1) {
            sem_post(semid, 0); // Освобождаем семафор при ошибке
            return 2;
        }
        printf("Received %d\n", arr[i]);
    }

    sem_post(semid, 0); // Освобождаем семафор после успешного чтения
    close(fd);
    semctl(semid, 0, IPC_RMID); // Удаляем семафор
    return 0;
}
