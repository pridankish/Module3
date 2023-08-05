#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

#define SIZE 5

sem_t semFile;

int main(int argc, char* argv[]) {
    sem_init(&semFile, 0, 1);
    int arr[SIZE];
    int fd = open("sum", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int i;
    sem_wait(&semFile);
    for(i = 0; i < SIZE; i++){
        if (read(fd, &arr[i], sizeof(int)) == -1){
            perror("Error reading from file");
            sem_post(&semFile);
            sem_destroy(&semFile);
            close(fd);
            exit(EXIT_FAILURE);
        }
        printf("Received %d\n", arr[i]);
    }
    sem_post(&semFile);
    close(fd);
    sem_destroy(&semFile);
    exit(EXIT_SUCCESS);
}