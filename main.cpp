#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include <csignal>

using namespace std;

char **tokenize(char*);
char ***split_words(char**);
void pwd(char**);
void echo(char*);
char* cd(char**, char* , char*);
void ls(char**);
int fw_bw(char**);
void pinfo(char**);
void search(char**,bool&);
void redirect(char *words,char *file,int operation);
void view_history(char **);
void add_history(char *);
void pipe(char *);

int foregroundid=-1;

char* input(){
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    if((nread = getline(&line, &len, stdin)) == -1){
        // handling ctrl d/z/c
        if (feof(stdin)) {
            exit(0);
        }
        // not able to read properly
        else {
            perror("Error while reading the input");
            exit(0);
        }
    }
    return line;
}

void free_memory(char **commands, char ***split_commands) {
    for (int i = 0; commands[i]; i++) {
        for (int j = 0; split_commands[i][j]; j++) {
            free(split_commands[i][j]);  // Free each word
        }
        free(split_commands[i]);  // Free the array of words for each command
        free(commands[i]);        // Free the duplicated command
    }
    free(split_commands);  // Free the array of commands
    free(commands);        // Free the array of semicolon-separated commands
}

void signalHandler(int signum) {
    if(signum==SIGINT){
        if(foregroundid>0){
            kill(foregroundid,SIGINT);
        }
    }
    else if(signum==SIGTERM){
        exit(0);
    }
    else if(signum==SIGTSTP){
        if (foregroundid > 0) {
            kill(foregroundid, SIGTSTP);
        }
        signal(SIGTSTP, SIG_IGN);
    }
}

int main(){
    umask(0);
    signal(SIGINT, signalHandler);
    string user;
    user = getenv("USER");
    char system[100];
    gethostname(system, 100);
    char root[1000];
    getcwd(root, 1000);
    string sroot(root);
    char *oldpwd = nullptr; 

    // open terminal until exited
    while(true){
        // print system and username
        cout << user << "@" << system << ":";
        char cwd[1000];
        getcwd(cwd, 1000);
        string scwd(cwd);

        // print the path
        if(scwd == sroot){
            cout << "~$";
        }    
        else if(scwd.find(sroot) != string::npos){
            cout << "~" << scwd.substr(sroot.length()) << "$";
        }
        else{
            cout << scwd << "$";
        }

        // get input using input function doubt( assumption : one line input)
        char *line  = input();
        if(!isspace((unsigned char)*line)){
            add_history(line);
        }
        else{
            continue;
        }

        // tokenize
        // first remove extra spaces and then tokenize the line to semi colon then pipe then redirection and the words
        char **commands = tokenize(line);

        // now tokenize words for each command
        char ***words = split_words(commands);

        // now look for command 
        for(int i = 0; commands[i]; i++){

            if(words[i][1] && words[i][1][strlen(words[i][1])-1]=='\n'){
                words[i][1][strlen(words[i][1])-1]='\0';
            }
            if(words[i][0] && words[i][0][strlen(words[i][0])-1]=='\n'){
                words[i][0][strlen(words[i][0])-1]='\0';
            }


            char *pos = strchr(commands[i], '|');
            if (pos != nullptr) {
                pipe(commands[i]);
                break;
            }

            pos = strchr(commands[i],'<');
            if ( pos != nullptr && strchr(commands[i],'>')!= nullptr ) {
                // Split command and file
                *pos = '\0';
                char *file = strtok(pos + 1, " ");
                redirect(commands[i], file, 4);
                break;
            }
            pos = strstr(commands[i], ">>");
            if (pos != nullptr) {
                // Split command and file
                *pos = '\0';
                char *file = strtok(pos + 2, " ");
                redirect(commands[i], file, 3);
                break;
            }
            pos = strchr(commands[i], '>');
            if (pos != nullptr) {
                // Split command and file
                *pos = '\0';
                char *file = strtok(pos + 1, " ");
                redirect(commands[i], file, 1);
                break;
            }
            pos = strchr(commands[i], '<');
            if (pos != nullptr) {
                // Split command and file
                *pos = '\0';
                char *file = strtok(pos + 1, " ");
                redirect(commands[i], file, 2);
                break;
            }
            if(strcmp(words[i][0], "pwd") == 0 ){
                pwd(words[i]);
            }
            else if(strcmp(words[i][0], "echo") == 0 ){
                echo(commands[i]);
            }
            else if(strcmp(words[i][0], "cd") == 0 ){
                oldpwd = cd(words[i], root , oldpwd);
            }
            else if(strcmp(words[i][0], "ls") == 0 ){
                ls(words[i]);
            }
            else if(strcmp(words[i][0], "pinfo") == 0 ){
                pinfo(words[i]);
            }
            else if(strcmp(words[i][0], "search") == 0 ){
                bool found = false;
                search(words[i],found);
                if(found){
                    cout<<"True\n";
                }
                else{
                    cout<<"False\n";
                }
            }
            else if(strcmp(words[i][0], "history") == 0){
                view_history(words[i]);
            }
            else if(strcmp(words[i][0], "exit") == 0 ){
                exit(0);
            }
            else{
                foregroundid = fw_bw(words[i]);
            }
        }

        // free up space
        free_memory(commands, words);
        free(line);
    }
    return 0;
}