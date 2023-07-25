#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define TRUE 1
#define FALSE 0
#define SIZE 20

int main(int argc, char* argv[]){
    char input[SIZE];
    int isFile = FALSE;

    DIR * dir = opendir(argv[1]);
    struct dirent * entry;

    while ((entry = readdir(dir)) != NULL){
        if (strcmp(entry->d_name, argv[2]) == 0){
            isFile = TRUE;
            break;
        }
    }

    if (isFile){
        char full_fn[200] = "";
        strcat(full_fn, argv[1]);
        strcat(full_fn, "/");
        strcat(full_fn, argv[2]);
        FILE * file_i = fopen(full_fn, "a");

        while (TRUE){
            printf("Enter string (-1 for quit): ");
            scanf("%s", input);
            if (strcmp(input, "-1") == 0) break;
            else fprintf(file_i, "%s ", input);
        }
        fclose(file_i);
    }
    else {
        printf("No such file as %s. Create? (Y/n): ", argv[2]);
        char yesno;
        scanf("%c", &yesno);
        if (yesno == 'y' || yesno == 'Y'){
            char full_fn[200] = "";
            strcat(full_fn, argv[1]);
            strcat(full_fn, "/");
            strcat(full_fn, argv[2]);
            FILE * file_i = fopen(full_fn, "a");

            while (TRUE){
                printf("Enter string (-1 for quit): ");
                scanf("%s", input);
                if (strcmp(input, "-1") == 0) break;
                else fprintf(file_i, "%s ", input);
            }
            fclose(file_i);
        }
        else
            return 0;
    }
    return 0;
}