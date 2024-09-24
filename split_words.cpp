#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>



char*** split_words(char **commands) {
    int size = 50;
    char ***split_commands = (char ***)calloc(size, sizeof(char**));
    if (!split_commands) {
        perror("unable to allocate memory");
        exit(0);
    }

    for (int i = 0; commands[i]; i++) {
        int total_word = 0;
        int word_array_size = 10;  // Initial size for the words array
        split_commands[i] = (char **)calloc(word_array_size, sizeof(char*));  // Initialize the words array
        if (!split_commands[i]) {
            perror("unable to allocate memory");
            exit(0);
        }

        char *command_copy = strdup(commands[i]); // Make a copy of the command for tokenization
        char *word = strtok(command_copy, " ");
        while (word) {
            if (total_word >= word_array_size) {
                word_array_size *= 2;
                split_commands[i] = (char **)realloc(split_commands[i], word_array_size * sizeof(char*));
                if (!split_commands[i]) {
                    perror("unable to re-allocate memory");
                    exit(0);
                }
            }

            if (word[0] == '"') {
            // Handling quoted strings
                char *temp = strtok(nullptr, "\"");
                if (temp && !isspace((unsigned char)*temp)) {
                    char *concatenated = (char *)malloc(strlen(word) + strlen(temp) + 2);  // +2 for space and null terminator
                    if (!concatenated) {
                        perror("unable to allocate memory");
                        exit(0);
                    }
                    strcpy(concatenated, word);
                    strcat(concatenated, " ");
                    strcat(concatenated, temp);
                    strcat(concatenated, "\"");
                    split_commands[i][total_word] = strdup(concatenated);
                    free(concatenated);
                } else {
                    // Handle the case where there's no closing quote
                    split_commands[i][total_word] = strdup(word);
                }
            } else {
                // Non-quoted words
                split_commands[i][total_word] = strdup(word);
            }
            // cout<<split_commands[i][total_word]<<endl;
            total_word++;
            word = strtok(nullptr, " ");
        }

        split_commands[i][total_word] = nullptr;  // Null-terminate the words array for this command
        free(command_copy); // Free the duplicated command copy
    }

    return split_commands;
}