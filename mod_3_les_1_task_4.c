#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROGRAM_NAME 20
#define MAX_ARGUMENTS 10
#define MAX_ARGUMENT_LENGTH 50

int main() {
    char programName[MAX_PROGRAM_NAME];
    char arguments[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH];
    int numberOfArguments = 0;

    while (1) {
        printf("Введите имя программы и аргументы ('q' для завершения): ");
        fflush(stdout);

        scanf("%s", programName);

        if (strcmp(programName, "q") == 0) {
            break;
        }

        numberOfArguments = 0;
        while (numberOfArguments < MAX_ARGUMENTS) {
            scanf("%s", arguments[numberOfArguments]);
            numberOfArguments++;
        }
        char *argv[MAX_ARGUMENTS + 2];
        argv[0] = programName;
        for (int i = 0; i < numberOfArguments; i++) {
            argv[i + 1] = arguments[i];
        }
        argv[numberOfArguments + 1] = NULL;

        execvp(programName, argv);

        perror("Ошибка при запуске программы");
        exit(EXIT_FAILURE);
    }
}
