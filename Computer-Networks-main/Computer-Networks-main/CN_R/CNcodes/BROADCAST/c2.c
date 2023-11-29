#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

int main()
{
 int fd;
 mkfifo("c22",0666);
 while(1){
  fd=open("c22",O_RDONLY);
  char s[100];
  read(fd,s,100);
  printf("%s",s);
  close(fd);
 }
}
