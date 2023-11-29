#include<sys/ipc.h>
#include<sys/shm.h>
#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>
#include <string.h>

#define semaphore1 "/semaphore1"
#define semaphore2 "/semaphore2"
#define x "x"
#define y "y"

char *str1,*str2;

void sigint(int sig)
{
  sem_unlink(semaphore1);
  sem_unlink(semaphore2);
  shmdt(str1);
  shmdt(str2);
  exit(0);
}

int main()
{
  signal(SIGINT,sigint);

  sem_t *s1=sem_open(semaphore1,O_RDWR);
  sem_t *s2=sem_open(semaphore2,O_RDWR);

  if (s1 == SEM_FAILED||s2==SEM_FAILED) {
      perror("sem_open(3) failed");
      exit(EXIT_FAILURE);
  }

  key_t key1=ftok(x,65);
  key_t key2=ftok(y,65);


  int shmid1=shmget(key1,1024,0666 | IPC_CREAT);
  int shmid2=shmget(key2,1024,0666 | IPC_CREAT);

  str1 = (char*) shmat(shmid1,NULL,0);
  str2 = (char*) shmat(shmid2,NULL,0);


  strcpy(str1,"0");
  strcpy(str2,"0");

  while(1)
  {
    sem_wait(s2);
    int dy=atoi(str2);
    int dx=dy+1;
    sprintf(str1,"%d",dx);
    printf("y-%d\n",dy );
    printf("x-%d\n",dx );
    fflush(stdout);
    sleep(1);
    sem_post(s1);
  }
}
