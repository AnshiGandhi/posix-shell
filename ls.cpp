#include <iostream>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <vector>

using namespace std;

char* to_cstr(string s)
{
  char *cstr=new char[s.size()+1];
  strcpy(cstr,s.c_str());
  return cstr;
}

void ls_long(char* fname)
{
  struct stat stat_temp;
  char* fpath = strdup(fname);
  strcat(fpath,"/");
  if(lstat(".",&stat_temp)==-1)
  {
    perror("Error in stating");

  }
  if ((stat_temp.st_mode & S_IFMT)==S_IFDIR)
  {
    cout<<"d";
  }
  else if ((stat_temp.st_mode & S_IFMT)==S_IFLNK)
  {
    cout<<"l";
  }
  else 
    cout<<"-";

  // Take care of permissions
  int val;
  val=(stat_temp.st_mode & ~S_IFMT);
  int perm[3];
  for(int i=0;i<3;i++)
  {
    perm[3-i]=val%8;
    val=val/8;
  }

  for(int i=1;i<=3;i++)
  {
    switch(perm[i])
    {

    case 0: cout<<"---"; 
            break;
    case 1: cout<<"--x"; 
            break;
    case 2: cout<<"-w-"; 
            break;
    case 3: cout<<"-wx"; 
            break;
    case 4: cout<<"r--"; 
            break;
    case 5: cout<<"r-x"; 
            break; 
    case 6: cout<<"rw-"; 
            break; 
    case 7: cout<<"rwx"; 
            break; 
    }
  }	
  //Unames and groups
  struct group *u_group=getgrgid((long)stat_temp.st_gid);
  struct passwd *user=getpwuid((long)stat_temp.st_uid);
  long int linx=(long)stat_temp.st_nlink;
  long fsize=stat_temp.st_size;
  struct tm *time_stamp=localtime(&stat_temp.st_mtime);

  //Printing data in order
  printf(" %3ld ",linx);
  printf(" %4s %4s ",user->pw_name,u_group->gr_name);
  printf(" %10ld ",fsize);

  //Date
  vector<string> months;
  months.push_back("Jan");
  months.push_back("Feb");
  months.push_back("Mar");
  months.push_back("Apr");
  months.push_back("May");
  months.push_back("Jun");
  months.push_back("Jul");
  months.push_back("Aug");
  months.push_back("Sep");
  months.push_back("Oct");
  months.push_back("Nov");
  months.push_back("Dec");
  printf(" %4d %s %2d ", time_stamp->tm_year+1900, to_cstr(months[time_stamp->tm_mon]),time_stamp->tm_mday);

  cout<<fname<<endl;
}

void ls_basic(char *fname,int a,int l){
    struct dirent **entry;
    int num_entries;

    if(strcmp(fname,"~")==0){
        char buf[1000];
        getcwd(buf,1000);
        fname =buf;
    }
    if(fname[strlen(fname)-1]=='\n'){
        fname[strlen(fname)-1]='\0';
    }

    num_entries=scandir(fname,&entry,0,alphasort);
    if(num_entries<0)
    {
      perror("Unable to scan directory");
      return;
    }

    // cout<<num_entries;
    // int count=1;
    for(int i=0;i<num_entries;i++)
    {
        if(l==0 && a==0 && entry[i]->d_name[0]!='.'){
            cout<<entry[i]->d_name<<'\n';
        }
        if(l==0 && a==1){
            cout<<entry[i]->d_name<<'\n';
        }
        if(l==1 && a==0 && entry[i]->d_name[0]!='.'){
            ls_long(entry[i]->d_name);
        }
        if(l==1 && a==1){
            ls_long(entry[i]->d_name);
        }
    }
}

void ls(char** words){
    int l=0,a=0;
    int count = 0;
    while (words[count] != nullptr) {
        count++;
    }
    int i=1;
    while(words[i]){
        if(words[i][strlen(words[i])-1]=='\n'){
            words[i][strlen(words[i])-1]='\0';
        }
        if(words[i][0] == '-'){
            for(int j=1;words[i][j]!='\0';j++)
            {
                char ch=words[i][j];
                switch(ch)
                {
                case 'l' : l=1;
                    break;
                case 'a' : a=1; 
                    break;
                default  : perror("Command not found");
                    exit(0);
                }
            }
        }
        i++;
    }
    i=1;
    count = 0;
    while(words[i]){
        if(words[i][0] != '-'){
            count++;
            cout<<words[i]<<" :\n";
            ls_basic(words[i],a,l);
        }
        i++;
    }
    if(count == 0){
        ls_basic((char *)".",a,l);
    }
}