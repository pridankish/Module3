#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
  char buffer[20];
  int file = open(argv[1], O_RDONLY);
  while (read(file, buffer, sizeof (buffer)) != 0) {
      printf("%s ", buffer);
  }
  close(file);
  return 0;
}