#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <signal.h>

int main()
{
  if(fork())
    printf("parent\n");
  else
      printf("child\n");
  //what does it mean by if(!fork())????
      
}
