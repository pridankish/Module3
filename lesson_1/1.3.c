#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int square(int a){
    return a * a;
}

int main(int argc, char* argv[]){
    pid_t pid = fork();

    switch (pid) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            printf("Дочерний класс:\n");
            for (int i = 1; i < argc / 2; i++)
                printf("%d\n", square(*argv[i] - 48));
            exit(EXIT_SUCCESS);
        default:
            printf("Родительский класс:\n");
            for (int i = argc / 2; i < argc; i++)
                printf("%d\n", square(*argv[i] - 48));
    }
    exit(EXIT_SUCCESS);
}