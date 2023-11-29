#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<signal.h>
#include<wait.h>
int val=0;
void hdfn()
{
  val+=10;
}
int main()
{
  signal(SIGCHLD,hdfn);
  pid_t c=fork();
  if(c==0)
  {
    val-=10;
    exit(0);
  }
  wait(NULL);
  printf("final value of val= %d",val);
  return 0;
}
