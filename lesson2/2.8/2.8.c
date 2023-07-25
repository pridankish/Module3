#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


int main(int argc, char* argv[]){
    int p1[2];
    if (pipe(p1) == -1){return 1;}

    int id = fork();

    if (id == -1){ return 2; }

    if (id == 0){
        // Child
        srand(time(NULL));
        int x = rand() % 10;
        if (write(p1[1], &x, sizeof(x)) == -1) { exit(3);}
        printf("Wrote: %d\n", x);
        if (read(p1[0], &x, sizeof (x)) == -1)  {return 4;}
        printf("Received by child (final): %d\n", x);
    } else {
        // Parent
        int y;
        if (read(p1[0], &y, sizeof(y)) == -1) {return 5;}
        printf("Received by parent: %d\n", y);
        y *= 2;
        if (write(p1[1], &y, sizeof (y)) == -1) {return 6;}

        FILE * file = fopen("test.txt", "w");
        fputs("Got from parent process:\n", file);
        fprintf(file, "%d", y);
        fclose(file);
    }
    close(p1[0]);
    close(p1[1]);
    return 0;
}