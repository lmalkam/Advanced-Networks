#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

struct msg{
  char name[100];
  char data[100];
}buff;
int main()
{
  buff.name[0]='\0';
  sprintf(buff.data,"%d",getpid());
  strcat(buff.data," is sending data");
  sleep(1);
  for(int i=0;i<2;i++)
  {
    write(1,&buff,sizeof(buff));
    sleep(2);
  }
  kill(getppid(),SIGUSR2);
}
