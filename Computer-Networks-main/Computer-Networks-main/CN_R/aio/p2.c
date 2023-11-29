#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

int main()
{
  while(1)
  {
    char out[]="message from P2\n";
    printf("%s\n",out);
    fflush(stdout);
    sleep(12);
  }
  return 0;
}
