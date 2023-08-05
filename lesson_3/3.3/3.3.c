#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int counter = 0;
int sigint_count = 0;

void sigint_handler(int signum) {
    printf("Received SIGINT (%d)\n", ++sigint_count);
    if (sigint_count >= 3) {
        printf("Received 3 SIGINT signals. Terminating the program.\n");
        exit(0);
    }
}

int main() {
    signal(SIGINT, sigint_handler);

    printf("Program is running in the background. Process ID: %d\n", getpid());
    unlink("text.txt");

    while (1) {
        FILE *file = fopen("text.txt", "a");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }
        fprintf(file, "%d\n", ++counter);
        fflush(file);
        fclose(file);
        sleep(1);
    }
}
