#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 5

int main(int argc, char* argv[]) {
    int arr[SIZE];
    int fd = open("sum", O_RDONLY);
    if (fd == -1) {
        return 1;
    }

    int i;
    for(i = 0; i < SIZE; i++){
        if (read(fd, &arr[i], sizeof(int)) == -1){
            return 2;
        }
        printf("Received %d\n", arr[i]);
    }
    close(fd);
}