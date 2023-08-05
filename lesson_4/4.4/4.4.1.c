#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>

#define SIZE 5

sem_t semFile;


int main(int argc, char* argv[])
{
    sem_init(&semFile, 0, 1);
    int arr[SIZE];
    srand(time(NULL));
    int i;
    for (i = 0; i < SIZE; i++){
        arr[i] = rand() % 100;
        printf("Generated %d\n", arr[i]);
    }

    int fd = open("sum", O_WRONLY);
    if (fd == -1){
        perror("Error opening file");
        sem_destroy(&semFile);
        exit(EXIT_FAILURE);
    }

    sem_wait(&semFile);
    
    if (write(fd, arr, sizeof (int) * SIZE) == -1) {
        sem_post(&semFile);
        sem_destroy(&semFile);
        close(fd);
        exit(EXIT_FAILURE);
    }

    sem_post(&semFile);
    close(fd);
    sem_destroy(&semFile);
    exit(EXIT_SUCCESS);
}