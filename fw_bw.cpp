#include <iostream>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>


using namespace std;

int fw_bw(char** words){
    
    int pid = fork();
    bool background = false;
    const char* lastElement = nullptr;
    int j;
    for ( j = 0; words[j] != nullptr; j++) {
        lastElement = words[j];
    }
    if(strcmp(lastElement,"&")==0 || strcmp(lastElement,"&\n")==0 || lastElement[strlen(lastElement)-1] == '&'){
        if(lastElement[strlen(lastElement)-1] == '&'){
            words[--j][strlen(lastElement)-1] = '\0';
        }
        background=true;
    }
    if(pid==0){
        execvp(words[0], words);
        perror("Invalid command");
        exit(0);
    }
    else if(pid>0 && !background){
        wait(NULL);
    }
    else if(pid>0){
        cout<<pid<<endl;
    }          
    else{
        perror("Failed to create child process");
        exit(0);
    }
    if(!background)
        return pid; 
    else{
        return -1;
    }
}