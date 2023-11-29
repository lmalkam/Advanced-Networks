#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<signal.h>

void hdfn()
{
  static int i=0;
  if(i==3)
    signal(SIGINT,SIG_DFL);
  else
  {
    printf("you have pressed CTRL+C %d times\n",i);
    i++;
  }
}
int main()
{
    signal(SIGINT,hdfn);
    while(1)
    {
      printf("waiting for you to press CTRL+C\n");
      sleep(1);
    }
}
