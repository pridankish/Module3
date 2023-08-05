#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SHARED_MEMORY_SIZE 4096

void sigintHandler(int signum);

int main() {
    signal(SIGINT, sigintHandler);

    int shmfd = shm_open("/shared_memory_file", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shmfd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shmfd, SHARED_MEMORY_SIZE) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    void* shared_memory = mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
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

        int min_pid = fork();
        if (min_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (min_pid == 0) {
            int min = data[1];
            for (int i = 2; i <= num_numbers; i++) {
                if (data[i] < min) {
                    min = data[i];
                }
            }
            data[num_numbers + 1] = min;
            exit(EXIT_SUCCESS);
        }

        int sum_pid = fork();
        if (sum_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (sum_pid == 0) {
            int sum = 0;
            for (int i = 1; i <= num_numbers; i++) {
                sum += data[i];
            }
            data[num_numbers + 2] = sum;
            exit(EXIT_SUCCESS);
        }


        int avg_pid = fork();
        if (avg_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (avg_pid == 0) {
            int sum = data[num_numbers + 2];
            double avg = (double)sum / num_numbers;
            data[num_numbers + 3] = (int)avg;
            exit(EXIT_SUCCESS);
        }

        int status;
        waitpid(min_pid, &status, 0);
        waitpid(sum_pid, &status, 0);
        waitpid(avg_pid, &status, 0);

        int min = data[num_numbers + 1];
        int sum = data[num_numbers + 2];
        int avg = data[num_numbers + 3];
        printf("Set %d: ", num_sets_processed + 1);
        for (int i = 1; i <= num_numbers; i++) {
            printf("%d ", data[i]);
        }
        printf("Min: %d Sum: %d Avg: %d\n", min, sum, avg);

        num_sets_processed++;
    }

    if (munmap(shared_memory, SHARED_MEMORY_SIZE) == -1) {
        perror("munmap");
    }

    if (close(shmfd) == -1) {
        perror("close");
    }
    if (shm_unlink("/shared_memory") == -1) {
        perror("shm_unlink");
    }

    printf("Processed %d sets of data\n", num_sets_processed);

    return 0;
}

void sigintHandler(int signum) {
    printf("\nReceived SIGINT. Exiting...\n");
}