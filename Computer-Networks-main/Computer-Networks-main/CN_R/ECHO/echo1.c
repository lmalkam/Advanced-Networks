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
    int fd=open("red",O_RDONLY);
    char buff[100];
    read(fd,buff,sizeof(buff));
    printf(" received ECHO1 %s\n",buff );


    close(fd);
    fgets(buff,sizeof(buff),stdin);
    fd=open("red",O_WRONLY);
    write(fd,buff,sizeof(buff));
    close(fd);

  }
}
