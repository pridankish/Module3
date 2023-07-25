#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define SIZE 5

int main(int argc, char* argv[])
{
    int arr[SIZE];
    srand(time(NULL));
    int i;
    for (i = 0; i < SIZE; i++){
        arr[i] = rand() % 100;
        printf("Generated %d\n", arr[i]);
    }

    int fd = open("sum", O_WRONLY);
    if (fd == -1){
        return 1;
    }

    if (write(fd, arr, sizeof (int) * SIZE) == -1) {
        return 2;
    }

    close(fd);

    return 0;
}