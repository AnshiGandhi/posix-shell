#include <iostream>
#include <cstdlib>
#include <string.h>
#include <unistd.h>

using namespace std;

void pwd(char** words){
    int count = 0;
    while (words[count] != nullptr) {
        count++;
    }
    if(count>1){
        perror("invalid command\n");
    }
    else{
        char buffer[500]; 
        getcwd(buffer,500);
        cout<<buffer<<endl;
    }
}