#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

int main()
{
 mkfifo("ffo",0666);
 int fd;
 while(1)
 {
   char s[100];
   fd=open("ffo",O_RDONLY);
   read(fd,s,100);
   printf("%s",s);
   close(fd);
   fd=open("ffo",O_WRONLY);
   fgets(s,100,stdin);
   write(fd,s,100);
   close(fd);
 }
}
