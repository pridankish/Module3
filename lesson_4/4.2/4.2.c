#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY_SEM 12345

int is_parent_modifying = 0;
int sem_id;

void sigusr1_handler(int signum) {
    is_parent_modifying = 1;
    printf("Доступ к файлу заблокирован для чтения.\n");
}

void sigusr2_handler(int signum) {
    is_parent_modifying = 0;
    printf("Доступ к файлу разблокирован для чтения.\n");
}

int main(int argc, char *argv[]) {
    int fd[2];
    pid_t pid;
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);

    if ((sem_id = semget(KEY_SEM, 1, IPC_CREAT | 0666)) == -1) {
        perror("Ошибка создания семафора.");
        exit(EXIT_FAILURE);
    }

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    } sem_init_val;
    sem_init_val.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_init_val) == -1) {
        perror("Ошибка инициализации семафора.");
        exit(EXIT_FAILURE);
    }

    if (pipe(fd) == -1) {
        perror("Ошибка создания канала.");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("Ошибка создания дочернего процесса.");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        sleep(3);
        close(fd[1]);
        FILE *output_file;

        output_file = fopen("output.txt", "w");
        if (output_file == NULL) {
            perror("Ошибка открытия файла.");
            exit(EXIT_FAILURE);
        }

        int received_number;
        raise(SIGUSR1);
        printf("Родительский процесс полученные числа: ");
        while (read(fd[0], &received_number, sizeof(int)) > 0) {
            printf("%d ", received_number);
            fprintf(output_file, "%d ", received_number);
        }
        printf("\nЧисла записаны в файл 'output.txt'\n");

        fclose(output_file);
        raise(SIGUSR2);

        close(fd[0]);
        wait(NULL);
    } else {
        close(fd[0]);
        srand(getpid());

        while (1) {
            struct sembuf sem_op;
            sem_op.sem_num = 0;
            sem_op.sem_op = -1;
            sem_op.sem_flg = 0;

            if (semop(sem_id, &sem_op, 1) == -1) {
                perror("Ошибка ожидания семафора.");
                exit(EXIT_FAILURE);
            }

            if (is_parent_modifying == 1)
                pause();
            else {
                struct sembuf sem_op;
                sem_op.sem_num = 0;
                sem_op.sem_op = 1;
                sem_op.sem_flg = 0;

                if (semop(sem_id, &sem_op, 1) == -1) {
                    perror("Ошибка установки семафора.");
                    exit(EXIT_FAILURE);
                }

                break;
            }
        }

        int val;
        FILE *input_file;
        input_file = fopen("output.txt", "r");
        if (input_file == NULL) {
            perror("Ошибка открытия файла.");
            exit(EXIT_FAILURE);
        }

        fscanf(input_file, "%d", &val);
        printf("Дочерний процесс прочитал число: ");
        printf("%d\n", val);
        fclose(input_file);

        val = val * 2;
        write(fd[1], &val, sizeof(int));

        sleep(1);
        close(fd[1]);
    }

    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("Ошибка удаления семафора.");
    }

    return 0;
}