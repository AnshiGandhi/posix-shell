#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>


using namespace std;

void add_history(char *line){
    int fd = open("/home/anshi/Documents/sem1/aos/assignments/assignment2/history.txt", O_RDWR | O_CREAT ,0644);
    if(fd < 0){
        perror("cannot open file");
        exit(0);
    }

    int file_size = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    char buff[file_size+1];
    read(fd,buff,file_size);
    buff[file_size] = '\0';

    int line_count = 0;
    for(int i=0;i<file_size+1;i++){
        if(buff[i] == '\n'){
            line_count++;
        }
    }

    if(line_count>=20){
        char *firstline = strchr(buff,'\n');
        if (firstline != nullptr) {
            firstline++;

            fd = open("/home/anshi/Documents/sem1/aos/assignments/assignment2/history.txt", O_WRONLY | O_TRUNC);
            if (fd < 0) {
                perror("cannot open file");
                exit(0);
            }

            write(fd, firstline, strlen(firstline));
         
        }
    }
    close(fd);

    fd = open("/home/anshi/Documents/sem1/aos/assignments/assignment2/history.txt",O_WRONLY | O_APPEND ,0644);
    if(fd < 0){
        perror("cannot open file");
        exit(0);
    }
    write(fd, line, strlen(line));
    close(fd);
}

void view_history(char **command){
    int n=20;
    if(command[1])
        n = atoi(command[1]);
    int fd = open("/home/anshi/Documents/sem1/aos/assignments/assignment2/history.txt", O_RDONLY | O_CREAT ,0644);
    if(fd < 0){
        perror("Error opening output file");
        exit(0);
    }

    if(n==20){
        char data[256];
        read(fd,data,256);
        cout<<data;
        return;
    }

    int file_size = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    char buff[file_size+1];
    read(fd,buff,file_size);
    buff[file_size] = '\0';

    int line_count = 0;
    for(int i=0;i<file_size+1;i++){
        if(buff[i] == '\n'){
            line_count++;
        }
    }

    if(line_count<n){
        char data[128];
        read(fd,data,128);
        cout<<data;
        return;
    }
    n = 20-n;
    char *firstline = buff;
    while(n--){
        firstline = strchr(firstline,'\n');
        if (firstline != nullptr) {
            firstline++;        
        }
    }
    cout<<firstline;
}