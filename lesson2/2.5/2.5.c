#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>


int main(int argc, char* argv[]){
   DIR *dir = opendir(argv[1]);

   struct dirent * entry;
   struct stat file_stat;

   if (dir == NULL){
       perror("Open error");
       return 1;
   }

   while ((entry = readdir(dir)) != NULL){
       stat(entry->d_name, &file_stat);

       if (S_ISREG(file_stat.st_mode)) printf("Common file ");
       else if (S_ISDIR(file_stat.st_mode)) printf("Directory ");
       else if (S_ISLNK(file_stat.st_mode)) printf("Link ");
       else printf("Unknown ");
       printf("%s\n", entry->d_name);
   }

   closedir(dir);
   return 0;
}