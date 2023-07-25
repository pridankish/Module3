#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define TRUE 1
#define SIZE 20

int main(int argc, char* argv[]){

    int file;
    char buffer[SIZE];

    file = open(argv[1], O_WRONLY | O_APPEND);

    while (TRUE) {
        printf("Enter string: ");
        fgets(buffer, sizeof(buffer), stdin);

        if (strcmp(buffer, "q\n") == 0){
            break;
        }

        write(file, buffer, strlen(buffer));
    }
    close(file);

    return 0;
}