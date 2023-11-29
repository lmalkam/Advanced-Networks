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
int pid;
struct sm
{
  int x;
  int y;
};
struct sm* mem;
void hdfn()
{
  printf("signal from p2 y=%d\n",mem->y);

  printf("updte x\n");
  scanf("%d",&mem->x);
  kill(pid,SIGUSR2);
}
int main()
{
  int i=getpid();
  mkfifo("red",0666);
  int fd=open("red",O_WRONLY);
  write(fd,&i,sizeof(int));
  close(fd);

  fd=open("red",O_RDONLY);
  read(fd,&pid,sizeof(int));
  close(fd);

  key_t k=ftok("shmfile",65);
  int shmid=shmget(k,1024,0666|IPC_CREAT);
  mem=(struct sm*)shmat(shmid,(void*)0,0);

  signal(SIGUSR1,hdfn);
  int f=0;
  mem->y=10;
  while(1)
  {
    if(f==0)
    {
      raise(SIGUSR1);
      f=1;

    }
    sleep(2);
  }
}
