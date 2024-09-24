#include <iostream>
#include <cstdlib>
#include <string.h>
#include <unistd.h>

using namespace std;
char *cd(char** words,char *root, char *oldpwd){
    int count = 0;
    while (words[count] != nullptr) {
        count++;
    }
    if(count>3 || (count==3 && !isspace((unsigned char)*words[2]))){
        perror("Invalid arguments for error handling\n");
    }
    else{
        // char current_dir[1024];
        if(count==1 || (count==2 && isspace((unsigned char)*words[1]))){
            char old[1000];
            getcwd(old, 1000);
            oldpwd = old;
            if(chdir(root)){
                perror("Error changing current directory");
            }
        }
        else{
            if(words[1][strlen(words[1])-1]=='\n'){
                words[1][strlen(words[1])-1]='\0';
            }
            if(strcmp(words[1],"-")==0){
                if(!oldpwd){
                    perror("old pwd not set");
                    return nullptr;
                }
                // char old[1000];
                // getcwd(old, 1000);
                chdir(oldpwd);
                oldpwd = nullptr;
            }
            else if(strcmp(words[1],"~")==0){
                char old[1000];
                getcwd(old, 1000);
                oldpwd = old;
                if(chdir(root)){
                    perror("Error changing current directory");
                }
            }
            else{
                char old[1000];
                getcwd(old, 1000);
                oldpwd = old;
                if(chdir(words[1])){
                    perror("Error changing current directory");
                }
            }
        }
    }
    return oldpwd;
}