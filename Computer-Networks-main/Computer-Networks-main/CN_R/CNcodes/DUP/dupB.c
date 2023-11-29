#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

int main()
{
 int c=0;
 int fd= open("Aspipe.txt",O_RDWR|O_APPEND);
 dup2(fd,1);
 c=fork();
 if(c==0)
 {
   char* args[] = {NULL};
   execvp("./p2",args);
 }
}
