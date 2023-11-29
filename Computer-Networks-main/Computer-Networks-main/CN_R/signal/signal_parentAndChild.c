#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<signal.h>

void hdfn(int signo)
{
  printf("ghi\n");
  if(signo==SIGUSR1)
    printf("signal sent by parent\n");
  else
      printf("signal sent by child\n");
}
int main()
{
  int c=fork();
  if(c>0)
  {
    signal(SIGUSR2,hdfn);
    while(1)
    {
      printf("abc\n");
      kill(c,SIGUSR1);
      sleep(2);
    }
  }
  else
  {
    signal(SIGUSR1,hdfn);
    while(1)
    {
      printf("def\n");
      kill(getppid(),SIGUSR2);
      sleep(2);
    }
  }
}
