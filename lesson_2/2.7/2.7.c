#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int fd[2];
    if (pipe(fd) == -1){
        printf("An error ocurred with opening the pipe\n");
        return 1;
    }
    int id = fork();
    if (id == -1){
        printf("An error ocurred with fork\n");
        return 4;
    }
    if (id == 0) {
        close(fd[0]);
        int arr[atoi(argv[1]) - 1];
        for (int i = 0; i < atoi(argv[1]); i++){
            arr[i] = rand() % 10;
            if (write(fd[1], &arr[i], sizeof(int)) == -1) {
                printf("An error ocurred with writing to the pipe\n");
                return 2;
            }
        }
        close(fd[1]);
    } else {
        close(fd[1]);
        int arr1[atoi(argv[1])- 1];
        for (int i = 0; i < atoi(argv[1]); i++){
            if (read(fd[0], &arr1[i], sizeof(int)) == -1){
                printf("An error ocurred with reading from the pipe\n");
                return 3;
            }
        }

        // вывод в консоль
        printf("Got from child process:\n");

        for (int i = 0; i < atoi(argv[1]); i++){
            printf("%d\n", arr1[i]);
        }
        close(fd[0]);


        // запись в файл

        FILE * file = fopen(argv[2], "w");
        fputs("Got from child process:\n", file);

        for (int i = 0; i < atoi(argv[1]); i++){
            fprintf(file, "%d\n", arr1[i]);
        }
        fclose(file);
        close(fd[0]);
    }
    return 0;
}