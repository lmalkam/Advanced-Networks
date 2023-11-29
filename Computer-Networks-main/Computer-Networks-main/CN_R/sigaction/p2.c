#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/poll.h>
#include<errno.h>

int main()
{
  system("pgrep p1 > pids.txt");
  FILE *fptr=fopen("pids.txt","r");
  int pid;
  fscanf(fptr,"%d",&pid);
  fscanf(fptr,"%d",&pid);
  printf("%d\n",pid );
  kill(pid,SIGUSR1);
}
