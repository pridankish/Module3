#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void exitFunction(){
    printf("Программа завершена!\n");
}

int main(int argc, char* argv[]){
    pid_t pid = fork();

    switch (pid){
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            printf("Дочерний процесс:\n");
            for (int i = 0; i < argc; i++) {
                printf("%d: %s\n", i, argv[i]);
            }
            exit(EXIT_SUCCESS);
        default:
            printf("Родительский процесс:\n");
            wait(&pid);
            for (int i = 0; i < argc; i++) {
                printf("%d: %s\n", i, argv[i]);
            }
    }
    atexit(exitFunction);
    exit(EXIT_SUCCESS);
}
