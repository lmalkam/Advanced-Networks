#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/wait.h>
#include<sys/types.h>

int main()
{
  while(1)
  {
    char out[]="message from P3\n";
    printf("%s\n",out);
    fflush(stdout);
    sleep(14);
  }
  return 0;
}
