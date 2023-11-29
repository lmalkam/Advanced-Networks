#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<pthread.h>
#include<sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
 int shmid;
 int *shm, *s;
 int sem1,sem2;
 int shm1,shm2;
 int *x;
 int *y;
 	sem1=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
 	sem2=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
 
	shm1=shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);
 	shm2=shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);
 
	x=(int*)shmat(shm1,NULL,0);
	y=(int*)shmat(shm2,NULL,0);

	semctl(sem1,0,SETVAL,0);
	semctl(sem2,0,SETVAL,1);

	struct sembuf p = { 0, -1, SEM_UNDO};
	struct sembuf v = { 0, +1, SEM_UNDO};
 int c=fork();
 if(c>0)
 {
    if(semop(sem2, &p, 1) < 0)
    {
       perror("semop p"); exit(13);
    }
    *x=*y+6;
    if(semop(sem1, &v, 1) < 0)
    {
        perror("semop p"); exit(14);
    }
    if(semop(sem2, &p, 1) < 0)
    {
        perror("semop p"); exit(13);
    }
    printf("%d \n",*y);
 }
 else
 {
    	if(semop(sem1, &p, 1) < 0)
        {
           perror("semop p"); exit(13);
        }
	printf("%d \n",*x);
	*y=*x+1;
	if(semop(sem2, &v, 1) < 0)
        {
          perror("semop p"); exit(14);
        }
 }
}
