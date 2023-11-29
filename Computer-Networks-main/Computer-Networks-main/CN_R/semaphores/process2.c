#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<signal.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int main()
{
  int s1,s2;
  s1=shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);
  s2=shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);

}
