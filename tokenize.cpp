#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>


char **tokenize(char *line){
    // remove extra spaces
    int len = strlen(line);
    for(int i = 0; i < len; i++){
        if(line[0] == ' '){
            for(i = 0; i < (len-1); i++)
                line[i] = line[i+1];
            line[i] = '\0';
            len--;
            i = -1;
            continue;
        }
        int j;
        if(line[i] == ' ' && line[i+1] == ' '){
            for(j = i; j < (len-1); j++){
                line[j] = line[j+1];
            }
            line[j] = '\0';
            len--;
            i--;
        }
    }

    // tokenize with ;
    int size = 16;
    int total_commands = 0;
    char **semicolon_sep_command = (char **)calloc(size, sizeof(char*));
    if (!semicolon_sep_command) {
        perror("unable to allocate memory");
        exit(0);
    }
    char *command = strtok(line, ";");
    while(command){
        if(total_commands >= size){
            size *= 2;
            semicolon_sep_command = (char **)realloc(semicolon_sep_command, size * sizeof(char*));
            if (!semicolon_sep_command) {
                perror("unable to re-allocate memory");
                exit(0);
            }
        }
        semicolon_sep_command[total_commands] = strdup(command); // Duplicate the command
        total_commands++;
        command = strtok(nullptr, ";");
    }
    semicolon_sep_command[total_commands] = nullptr; // Null-terminate the array

    return semicolon_sep_command;
}