#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHARED_MEMORY_SIZE 4096

void sigintHandler(int signum);

int main() {
    signal(SIGINT, sigintHandler);

    int shmid;
    key_t key = ftok("shared_memory_file", 1);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    shmid = shmget(key, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    void* shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    int* data = (int*)shared_memory;
    int num_sets_processed = 0;

    while (1) {
        int num_numbers = rand() % 10 + 1;
        data[0] = num_numbers;

        for (int i = 1; i <= num_numbers; i++) {
            data[i] = rand() % 100;
        }
        int pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            int max = data[1];
            int min = data[1];

            for (int i = 2; i <= num_numbers; i++) {
                if (data[i] > max) {
                    max = data[i];
                }
                if (data[i] < min) {
                    min = data[i];
                }
            data[0] = max;
            data[1] = min;

            exit(EXIT_SUCCESS);
        } else {
            int status;
            wait(&status);

            if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) {
                break;
            }

            int max = data[0];
            int min = data[1];

            printf("Set %d: ", num_sets_processed + 1);
            for (int i = 1; i <= num_numbers; i++) {
                printf("%d ", data[i]);
            }
            printf("Max: %d Min: %d\n", max, min);

            num_sets_processed++;
        }
    }
    shmdt(shared_memory);

    shmctl(shmid, IPC_RMID, NULL);

    printf("Processed %d sets of data\n", num_sets_processed);

    return 0;
}

void sigintHandler(int signum) {
    printf("\nReceived SIGINT. Exiting...\n");
}