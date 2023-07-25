#include <stdio.h>
#include <dirent.h>
#include "string.h"

#define TRUE 1
#define FALSE 0

int main(int argc, char* argv[]){

    char output[20];
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
        FILE* file_o = fopen(full_fn, "r");

        while (fscanf(file_o,"%s", output) != EOF){
            printf("%s ", output);
        }
        fclose(file_o);
    }

    else{
        printf("No such file as %s. Create? (Y/n): ", argv[2]);
        char yesno;
        scanf("%c", &yesno);
        if (yesno == 'y' || yesno == 'Y'){
            char full_fn[200] = "";
            strcat(full_fn, argv[1]);
            strcat(full_fn, "/");
            strcat(full_fn, argv[2]);
            FILE* file_o = fopen(full_fn, "a");

            while (fscanf(file_o,"%s", output) != EOF){
                printf("%s ", output);
            }
            fclose(file_o);
        }
        else if (yesno == 'n' || yesno == 'N')
            return 0;
    }
    return 0;
}