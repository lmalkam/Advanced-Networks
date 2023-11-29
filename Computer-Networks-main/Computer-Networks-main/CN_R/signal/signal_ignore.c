#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<signal.h>

int main()
{
  signal(SIGINT,SIG_IGN);
  while(1)
  {
    printf("please press CTRL+C\n");
    sleep(2);
  }
}
