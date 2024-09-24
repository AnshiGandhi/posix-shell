#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <cstring>
#include <cctype>
#include <unistd.h>
#include <cstdlib>

using namespace std;

char **tokenize(char*);

void pinfo(char** words) {
    pid_t current_pid = getpid();
    char path[100];

    if (words[1] == NULL) {
        // Handle case where no specific PID is provided
            DIR *proc_dir = opendir("/proc/");
    if (!proc_dir) {
        perror("Failed to open /proc directory");
        exit(1);
    }

    struct dirent *entry;
    while ((entry = readdir(proc_dir)) != nullptr) {
        // Check if the directory name is numeric (i.e., a process ID)
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            // Construct the path to the stat file
            char path[1024];
            snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);

            FILE *mem = fopen(path, "r");
            if (!mem) {
                perror("Error opening file");
                continue;
            }

            // Read the file content into a buffer
            char buffer[1024];  // Enough for a single line from /proc/[PID]/stat
            if (fgets(buffer, sizeof(buffer), mem) == nullptr) {
                perror("Reading error");
                fclose(mem);
                continue;
            }
            fclose(mem);

            // Tokenize the buffer
            int size = 16;
            int total_commands = 0;
            char **tokens = (char **)calloc(size, sizeof(char*));
            if (!tokens) {
                perror("Unable to allocate memory");
                exit(1);
            }
            char *command = strtok(buffer, " ");
            while (command) {
                if (total_commands >= size) {
                    size *= 2;
                    tokens = (char **)realloc(tokens, size * sizeof(char*));
                    if (!tokens) {
                        perror("Unable to re-allocate memory");
                        exit(1);
                    }
                }
                tokens[total_commands] = strdup(command); // Duplicate the command
                total_commands++;
                command = strtok(nullptr, " ");
            }
            tokens[total_commands] = nullptr; // Null-terminate the array

            int pid = atoi(tokens[0]);
            int ppid = atoi(tokens[3]);

            // Check if the process has the same parent process ID as the current shell
            if (ppid == current_pid || pid == current_pid) {
                cout << "pid -- " << pid << endl;
                cout << "ppid -- " << ppid << endl;
                cout << "Process status -- " << tokens[2];
                if (strcmp(tokens[4], tokens[7]) == 0) {
                    cout << "+" << endl;
                } else {
                    cout << endl;
                }
                cout << "Memory -- " << tokens[22] << endl;
                cout << "Executable Path -- " << tokens[1] << endl;
            }

            // Free memory allocated for tokens
            for (int i = 0; i < total_commands; ++i) {
                free(tokens[i]);
            }
            free(tokens);
        }
    }

    closedir(proc_dir);    }
    else {
        // Handle case where words[1] is not NULL (a specific PID is provided)
        strcpy(path, "/proc/");
        strcat(path, words[1]);  // Add the PID from words[1]
        strcat(path, "/stat");
        FILE *mem = fopen(path, "r");
        if (mem == nullptr) {
            perror("Error opening file");
            exit(0);
        }

        // Read the file content into a buffer
        char buffer[1024];  // Enough for a single line from /proc/[PID]/stat
        if (fgets(buffer, sizeof(buffer), mem) == nullptr) {
            perror("Reading error");
            fclose(mem);
            exit(0);
        }
        fclose(mem);

        // Tokenize the buffer
        int size = 16;
        int total_commands = 0;
        char **tokens = (char **)calloc(size, sizeof(char*));
        if (!tokens) {
            perror("unable to allocate memory");
            exit(0);
        }
        char *command = strtok(buffer, " ");
        while (command) {
            if (total_commands >= size) {
                size *= 2;
                tokens = (char **)realloc(tokens, size * sizeof(char*));
                if (!tokens) {
                    perror("unable to re-allocate memory");
                    exit(0);
                }
            }
            tokens[total_commands] = strdup(command); // Duplicate the command
            total_commands++;
            command = strtok(nullptr, " ");
        }
        tokens[total_commands] = nullptr; // Null-terminate the array

        int ppid = atoi(tokens[3]);
        cout << "pid -- " << atoi(words[1]) << endl;
        cout << "ppid -- " << ppid << endl;
        cout << "Process status -- " << tokens[2];
        if (strcmp(tokens[4], tokens[7]) == 0) {
            cout << "+" << endl;
        } else {
            cout << endl;
        }
        cout << "Memory -- " << tokens[22] << endl;
        cout << "Executable Path -- " << tokens[1] << endl;

        // Free memory allocated for tokens
        for (int i = 0; i < total_commands; ++i) {
            free(tokens[i]);
        }
        free(tokens);
    }
}

