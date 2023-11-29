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
  printf("signal from p1 value x=%d\n",mem->x);
  printf("updte y\n");
  int val;
  scanf("%d",&val);
  mem->y=val;
  kill(pid,SIGUSR1);
}
int main()
{
  int i=getpid();
  mkfifo("red",0666);
  int fd=open("red",O_RDONLY);
  read(fd,&pid,sizeof(int));
  close(fd);

  fd=open("red",O_WRONLY);
  write(fd,&i,sizeof(int));
  close(fd);

  key_t k=ftok("shmfile",65);
  int shmid=shmget(k,1024,0666|IPC_CREAT);
  mem=(struct sm*)shmat(shmid,(void*)0,0);
  mem->x=5;
  signal(SIGUSR2,hdfn);
  while(1)
  {
    sleep(2);
  }
}
