#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

bool fileIsOpen = false;

void sigint_quit_handler(int sig) {
    if (fileIsOpen) {
        printf("Program is working with file!\n");
    } else {
        exit(EXIT_SUCCESS);
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigint_quit_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    int counter = 0;
    printf("Program is running in the background. Process ID: %d\n", getpid());
    unlink("text.txt");
    while (1) {
        FILE *file = fopen("text.txt", "aw");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }
        fileIsOpen = true;
        fprintf(file, "%d\n", ++counter);
        fflush(file);
        fileIsOpen = false;
        sleep(1);
    }
}