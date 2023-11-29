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
  mkfifo("ffo1",0666);
  mkfifo("ffo2",0666);
  pthread_t p1;
  pthread_t p2;
  char a1[]="ffo1";
  char a2[]="ffo2";
  int x=pthread_create(&p1,NULL,reading,(void*)a2);
  int y=pthread_create(&p2,NULL,writing,(void*)a1);

  pthread_join(p1,NULL);
  pthread_join(p2,NULL);
 exit(0);
}
