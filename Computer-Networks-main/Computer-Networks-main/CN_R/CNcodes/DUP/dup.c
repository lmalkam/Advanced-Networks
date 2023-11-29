#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

int main()
{
 int c=0;
 char buf[101];
 int pp[2];
 pipe(pp);
 int fd=dup(1);
 int fd1=open("Aspipe.txt",O_RDWR);
 dup2(fd1,1);
 c=fork();
 if(c>0)
 {
   close(pp[1]);
   read(pp[0],buf,100);
   dup2(fd,1);
   printf("%s",buf);
 }
 else
 {
   close(pp[0]);
   char* args[] = {NULL};
   execvp("./p2",args);
 }
}
