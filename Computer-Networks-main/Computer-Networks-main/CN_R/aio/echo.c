#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>

int main()
{
  while(1)
  {
    char m[]="Echo this message to cclient\n";
    printf("%s\n",m);
    fflush(stdout);
    sleep(rand()%10+10);
  }
  return 0;
}
