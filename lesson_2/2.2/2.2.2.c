#include <stdio.h>

int main(int argc, char* argv[]){

    char output[20];

    FILE* file_o = fopen(argv[1], "r");

    while (fscanf(file_o,"%s", output) != EOF){
        printf("%s ", output);
    }
    fclose(file_o);

    return 0;
}