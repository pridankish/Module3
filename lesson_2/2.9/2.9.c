#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_ARGS 10

void execute_command(int input_fd, int output_fd, char* cmd[]) {
    if (cmd[0] == NULL) {
        return; 
    }

    if (output_fd != -1) {
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("Pipe creation failed");
            exit(1);
        }
        output_fd = pipefd[1];
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        if (input_fd != -1) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        if (output_fd != -1) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        execvp(cmd[0], cmd);
        perror("Execution failed");
        exit(1);
    } else {
        if (input_fd != -1) {
            close(input_fd);
        }
        if (output_fd != -1) {
            close(output_fd);
        }

        int status;
        waitpid(pid, &status, 0);
    }
}

int main(int argc, char* argv[]) {
    int input_fd = -1;
    int output_fd = -1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "<") == 0 && i + 1 < argc) {
            input_fd = open(argv[i + 1], O_RDONLY);
            i++; // Skip the file name after "<"
        } else if (strcmp(argv[i], ">") == 0 && i + 1 < argc) {
            output_fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            i++;
        } else {
            char* cmd[MAX_ARGS + 1];
            int num_args = 0;
            while (argv[i] != NULL && strcmp(argv[i], "|") != 0 && num_args < MAX_ARGS) {
                cmd[num_args] = argv[i];
                num_args++;
                i++;
            }
            cmd[num_args] = NULL;

            execute_command(input_fd, output_fd, cmd);
            input_fd = -1;
            output_fd = -1;
        }
    }

    return 0;
}