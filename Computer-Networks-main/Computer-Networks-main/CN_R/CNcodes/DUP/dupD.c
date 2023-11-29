#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

int main()
{
 int c=0;
 int buf[100];
 int pp[2];
 pipe(pp);
 int fd= open("Aspipe.txt",O_RDWR|O_APPEND);
 dup2(fd,1);
 dup2(fd,pp[0]);
 c=fork();
 if(c>0)
 {
   close(pp[1]);
   read(pp[0],buf,100);
 }
 else
 {
   char* args[] = {NULL};
   execvp("./p2",args);
 }
}
