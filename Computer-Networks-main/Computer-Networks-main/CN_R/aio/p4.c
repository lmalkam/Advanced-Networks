#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<string.h>

int main()
{
  sleep(10);
  while(1)
  {
    kill(getpid(),SIGUSR1);
    sleep(18);
  }
  return 0;
}
