#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <dirent.h>

using namespace std;

// Use a static boolean to keep track of whether the target has been found
void search(char** words, bool& found) {
    struct dirent** entry;
    int num_entries;

    if(!words[1]){
        return;
    }

    // Remove newline character if it exists
    if (words[1][strlen(words[1]) - 1] == '\n') {
        words[1][strlen(words[1]) - 1] = '\0';
    }

    // Scan the current directory
    num_entries = scandir(".", &entry, nullptr, alphasort);
    if (num_entries < 0) {
        perror("Unable to scan directory");
        return;
    }

    for (int i = 0; i < num_entries; ++i) {
        if (strcmp(entry[i]->d_name, ".") == 0 || strcmp(entry[i]->d_name, "..") == 0) {
            continue;
        }

        if (strcmp(entry[i]->d_name, words[1]) == 0) {
            found = true;
            break;
        }

        if (entry[i]->d_type == DT_DIR) {
            // Prepare new directory path
            char* newDir = new char[strlen(entry[i]->d_name) + 2];
            strcpy(newDir, entry[i]->d_name);
            strcat(newDir, "/");

            // Change to the subdirectory
            if (chdir(newDir) == 0) {
                search(words, found);
                chdir("..");  // Change back to the parent directory
            } else {
                perror("Failed to change directory");
            }

            delete[] newDir;

            if (found) {
                break;
            }
        }

        free(entry[i]);
    }

    free(entry);
}

