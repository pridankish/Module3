#include <stdio.h>
#include <string.h>

#define TRUE 1
#define SIZE 20

int main(int argc, char* argv[]){
    char input[SIZE];

    FILE * file_i = fopen(argv[1], "a");

    while (TRUE){
        printf("Enter string (-1 for quit): ");
        scanf("%s", input);
        if (strcmp(input, "-1") == 0) break;
        else fprintf(file_i, "%s ", input);
    }
    fclose(file_i);

    return 0;
}