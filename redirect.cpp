#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <sys/wait.h>

using namespace std;

void redirect(char *command, char *file, int operation) {
    if (file[strlen(file) - 1] == '\n') {
        file[strlen(file) - 1] = '\0';
    }

    int pid = fork();
    if (pid == 0) {
        if (operation == 2) { // Input redirection (<)
            int fd0 = open(file, O_RDONLY);
            if (fd0 < 0) {
                perror("Error opening input file");
                exit(0);
            }
            dup2(fd0, STDIN_FILENO);
            close(fd0);
        } else if (operation == 1) { // Output redirection (>)
            int fd1 = creat(file, 0644);
            if (fd1 < 0) {
                perror("Error opening output file");
                exit(0);
            }
            dup2(fd1, STDOUT_FILENO);
            close(fd1);
        } else if(operation == 3) {
            int fd2 = open(file, O_WRONLY | O_APPEND | O_CREAT ,0644);
            if(fd2 < 0){
                perror("Error opening output file");
                exit(0);
            }
            dup2(fd2, STDOUT_FILENO);
            close(fd2);
        } else if(operation == 4){
            char *pos = strchr(file, '>');
            char *outfile;
            if (pos != nullptr) {
                // Split input and output file
                *pos = '\0';
                outfile = strtok(pos + 1, " ");
            }
            int fd0 = open(file, O_RDONLY);
            if (fd0 < 0) {
                perror("Error opening input file");
                exit(0);
            }
            dup2(fd0, STDIN_FILENO);
            close(fd0);
            int fd1 = creat(outfile, 0644);
            if (fd1 < 0) {
                perror("Error opening output file");
                exit(0);
            }
            dup2(fd1, STDOUT_FILENO);
            close(fd1);
        }

        char *args[64];
        int i = 0;
        args[i] = strtok(command, " ");
        while (args[i] != nullptr) {
            i++;
            args[i] = strtok(nullptr, " ");
        }
        args[i] = nullptr;

        execvp(args[0], args); // Execute the command

        perror("failed to run command");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("Failed to create child process");
        exit(1);
    }
}