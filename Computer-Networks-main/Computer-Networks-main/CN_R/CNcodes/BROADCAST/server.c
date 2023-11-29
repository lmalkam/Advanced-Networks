#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

int main()
{
 mkfifo("c11",0666);
 mkfifo("c22",0666);
 int fd;
 while(1)
 {
   int fd1,fd2;
   fd1=open("c11",O_WRONLY);
   char s[100];
   fgets(s,100,stdin);
   write(fd1,s,100);
   close(fd1);
   fd2=open("c22",O_WRONLY);
   write(fd2,s,100);
   close(fd2);
 }
}
