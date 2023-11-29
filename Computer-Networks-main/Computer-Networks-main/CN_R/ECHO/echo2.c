#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
  mkfifo("red",0666);
  while(1)
  {
    int fd=open("red",O_WRONLY);
    char buff[100];
    fgets(buff,sizeof(buff),stdin);
    write(fd,buff,sizeof(buff));
    close(fd);
    fd=open("red",O_RDONLY);
    read(fd,buff,sizeof(buff));
    printf("received ECHO2%s\n",buff);
    close(fd);
  }
}
