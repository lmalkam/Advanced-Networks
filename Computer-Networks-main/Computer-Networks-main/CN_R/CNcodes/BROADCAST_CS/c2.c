#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<pthread.h>
void* reading(void *ptr)
{
 char *message;
 message = (char *) ptr;
 int fd;
  while(1)
  {	
	char s[100];
	fd=open(message,O_RDONLY);
	read(fd,s,100);
	printf("%s",s);
  }
}

void* writing(void *ptr)
{
  char *message;
  message = (char *) ptr;
  int fd;
  while(1)
  {
	char s[100];
	fd=open(message,O_WRONLY);
	fgets(s,100,stdin);
	write(fd,s,100);
  }
}

int main()
{
  mkfifo("ffo2",0666);
  mkfifo("ffo5",0666);
  pthread_t p1,p2;
  char a1[]="ffo5";
  char a2[]="ffo2";
  pthread_create(&p2,NULL,writing,(void*)a2);
  pthread_create(&p1,NULL,reading,(void*)a1);

  pthread_join(p2,NULL);
  pthread_join(p1,NULL);
 exit(0);
}
