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

#define semaphore1 "/semaphore1"
#define semaphore2 "/semaphore2"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

int main()
{
  sem_t *s1=sem_open(semaphore1,O_CREAT | O_EXCL,SEM_PERMS,0);
  sem_t *s2=sem_open(semaphore2,O_CREAT | O_EXCL,SEM_PERMS,1);

  if (s1 == SEM_FAILED||s2==SEM_FAILED) {
      perror("sem_open(3) failed");
      exit(EXIT_FAILURE);
  }

  if(sem_close(s1)<0||sem_close(s2)<0)
  {
    perror("sem_close failed");
  }
  return 0;

}
