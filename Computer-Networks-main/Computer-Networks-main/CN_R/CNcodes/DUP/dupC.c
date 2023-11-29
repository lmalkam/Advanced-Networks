#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

int main()
{
  printf("\n");
  int fd=open("Aspipe.txt",O_RDWR|O_APPEND);
  int f=dup(1);
  dup2(fd,1);
  printf("file output \n");
  dup2(f,1);
  printf("Screen output \n");
}
