#include <stdio.h>
#include <unistd.h>
#include <signal.h>


int main() {
    int counter = 0;
    printf("Program is running in the background. Process ID: %d\n", getpid());
    unlink("text.txt");
    signal(SIGINT, SIG_IGN);
    while (1) {
        FILE *file = fopen("text.txt", "aw");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }
        fprintf(file, "%d\n", ++counter);
        fflush(file);
        sleep(1);
    }
}