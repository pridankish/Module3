#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void listFiles(const char* path){
    DIR* dir;
    struct dirent * entry;

    dir = opendir(path);
    if (dir == NULL) {
        printf("Open error");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)) {
            continue;
        }

        if (entry->d_name == DT_DIR) {
            printf("[DIR]%s\n", entry->d_name);
        } else {
            printf("[FILE]%s\n", entry->d_name);
        }
    }
    closedir(dir);
}


int main(){
    char path[150];

    printf("Введите путь к каталогу: ");
    scanf("%s", path);

    listFiles(path);

    int choice;

    printf("\nВыберите действие:\n");
    printf("1. Перейти в подкаталог\n");
    printf("2. Вернуться на один уровень назад\n");
    printf("0. Выход из программы\n");
    printf("Ваш выбор: ");
    scanf("%d", &choice);

    while (choice != 0)
    {
        switch (choice) {
            case 1: {
                char subdir[150];
                printf("Введите имя подкаталога");
                scanf("%s", subdir);

                size_t new_path_size = strlen(path) + 1 + strlen(subdir) + 1;
                char* new_path = (char*) malloc(new_path_size);

                if (new_path == NULL){
                    printf("Ошибка выделения памяти.\n");
                    exit(EXIT_FAILURE);
                }
                snprintf(new_path, new_path_size, "%s/%s", path, subdir);

                DIR * temp_dir = opendir(new_path);
                if (temp_dir != NULL){
                    closedir(temp_dir);
                    strcpy(path, new_path);
                    listFiles(path);
                } else {
                    printf("Подкаталог '%s' не существует или это не каталог.\n", subdir);
                }

                free(new_path);
                break;
            }
            case 2:{
                char* last_slash = strrchr(path, '/');
                if (last_slash != NULL){
                    *last_slash = '\0';
                } else
                {
                    printf("Невозможно вернуться на уровень назад.\n");
                }
                listFiles(path);
                break;
            }
            case 0:
                break;
            default:
                printf("Некорректный ввод. Введите 0, 1 или 2.\n");
                break;
        }
        printf("\nВыберите действие:\n");
        printf("1. Перейти в подкаталог\n");
        printf("2. Вернуться на один уровень назад\n");
        printf("0. Выход из программы\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);
    }
}