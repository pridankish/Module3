#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int sum = 0;

    for (int i = 1; i < argc; i++) {
        int num = atoi(argv[i]);
        sum += num;
    }
    printf("\x1b[32m>\x1b[0m ");
    printf("Sum of arguments: %d\n", sum);

    return 0;
}