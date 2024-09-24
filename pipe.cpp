#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

using namespace std;

void redirect(char *words,char *file,int operation);

char* trimWhitespace(char* str) {
    while(isspace(*str)) str++;
    if(*str == 0) return str;

    char *end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;
    *(end+1) = 0;

    return str;
}

int countPipes(char *line){
    int count = 0;
    char *temp = line;
    while (*temp != '\0') {
        if (*temp == '|') count++;
        temp++;
    }
    return count + 1;
}

void pipe(char *command) {
    int numCommands = countPipes(command);
    int numPipes = 2 * (numCommands - 1);
    int status;
    int pipefds[numPipes];

    for (int i = 0; i < numPipes; i += 2) {
        if (pipe(pipefds + i) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    int commandIndex = 0;
    char *cmd = strtok(command, "|");

    while (cmd != nullptr) {
        cmd = trimWhitespace(cmd);

        int pid = fork();
        if (pid == 0) {
            if (commandIndex < numCommands - 1) {
                if (dup2(pipefds[2 * commandIndex + 1], 1) < 0) {
                    perror("dup2 - writing");
                    exit(EXIT_FAILURE);
                }
            }
            if (commandIndex > 0) {
                if (dup2(pipefds[2 * commandIndex - 2], 0) < 0) {
                    perror("dup2 - reading");
                    exit(EXIT_FAILURE);
                }
            }

            for (int i = 0; i < numPipes; i++) {
                close(pipefds[i]);
            }


            char *pos = strstr(cmd, ">>");
            if (pos != nullptr) {
                // Split command and file
                *pos = '\0';
                char *file = strtok(pos + 2, " ");
                redirect(cmd, file, 3);
                return;
            }
            pos = strchr(cmd, '>');
            if (pos != nullptr) {
                // Split command and file
                *pos = '\0';
                char *file = strtok(pos + 1, " ");
                redirect(cmd, file, 1);
                return;
            }
            pos = strchr(cmd, '<');
            if (pos != nullptr) {
                // Split command and file
                *pos = '\0';
                char *file = strtok(pos + 1, " ");
                redirect(cmd, file, 2);
                return;
            }


            char *args[64];
            int k = 0;
            args[k] = strtok(cmd, " ");
            while (args[k] != nullptr) {
                k++;
                args[k] = strtok(nullptr, " ");
            }
            args[k] = nullptr;

            if (execvp(args[0], args) < 0) {
                perror(args[0]);
                exit(EXIT_FAILURE);
            }
        } else if (pid < 0) { // Fork failed
            perror("fork");
            exit(EXIT_FAILURE);
        }

        // Parent process: move to the next command
        cmd = strtok(nullptr, "|");
        commandIndex++;
    }

    // Parent process: close all pipes and wait for children
    for (int i = 0; i < numPipes; i++) {
        close(pipefds[i]);
    }

    for (int i = 0; i < numCommands; i++) {
        wait(&status);
    }
}
