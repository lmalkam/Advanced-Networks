#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<pthread.h>
char *fifo[10];
void *function_1(void* msg)
{
while(1)
{
 int fd1=open(fifo[1],O_RDONLY);
 char s[100];
 read(fd1,s,100);
 close(fd1);
 int fd2,fd3;
 fd2=open(fifo[5],O_WRONLY);
 write(fd2,s,100);
 close(fd2);
 fd3=open(fifo[6],O_WRONLY);
 write(fd3,s,100);
 close(fd3);
}
}

void *function_2(void* msg)
{
while(1)
{
 int fd1=open(fifo[2],O_RDONLY);
 char s[100];
 read(fd1,s,100);
 close(fd1);
 int fd2,fd3;
 fd2=open(fifo[4],O_WRONLY);
 write(fd2,s,100);
 close(fd2);
 fd3=open(fifo[6],O_WRONLY);
 write(fd3,s,100);
 close(fd3);
}
}

void *function_3(void* msg)
{
while(1)
{
 int fd1=open(fifo[3],O_RDONLY);
 char s[100];
 read(fd1,s,100);
 close(fd1);
 int fd2,fd3;
 fd2=open(fifo[4],O_WRONLY);
 write(fd2,s,100);
 close(fd2);
 fd3=open(fifo[5],O_WRONLY);
 write(fd3,s,100);
 close(fd3);
}
}

int main()
{
  mkfifo("ffo1",0666);
  mkfifo("ffo2",0666);
  mkfifo("ffo3",0666);
  mkfifo("ffo4",0666);
  mkfifo("ffo5",0666);
  mkfifo("ffo6",0666);
  fifo[1]="ffo1";
  fifo[2]="ffo2";
  fifo[3]="ffo3";
  fifo[4]="ffo4";
  fifo[5]="ffo5";
  fifo[6]="ffo6";
  pthread_t p1,p2,p3;
   pthread_create(&p1,NULL,function_1,NULL);
   pthread_create(&p2,NULL,function_2,NULL);
   pthread_create(&p3,NULL,function_3,NULL);

   pthread_join(p1,NULL);
   pthread_join(p2,NULL);
   pthread_join(p3,NULL);
 exit(0);
}
/*while(1)
 {	
  for(int i=1;i<=3;i++)
  {
	int fd=open(fifo[i],O_RDONLY);
	
	char s[100];
	printf("%s\n",fifo[i]);
	if(read(fd,s,100)>0)
	{	
		close(fd);
	   for(int j=4;j<=6;j++)
	   {
		if(i+3!=j)
		{
		  int fd=open(fifo[j],O_WRONLY);
		  write(fd,s,100);
		  close(fd);
		}
           }
	}
	close(fd);
  }
 }*/
/*pthread_t p[6];
  for(int i=0;i<6;i++)
  {
	pthread_create(&p[i],NULL,function,NULL);
  }

  for(int i=0;i<6;i++)
  {
	pthread_join(p[i],NULL);
  }*/

