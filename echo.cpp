#include <iostream>
#include <cstdlib>
#include <string.h>
#include <unistd.h>

using namespace std;

void echo(char* words){
    char *a = words + 5;
    cout<<a<<endl;
}