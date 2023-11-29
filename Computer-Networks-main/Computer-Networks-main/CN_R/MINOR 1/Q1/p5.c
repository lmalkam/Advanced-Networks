#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

int main()
{
  int n;
  while(1)
  {
    char buff[100];
    memset(buff,0,sizeof(buff));
    read(0,&buff,100);
    //buff[strlen(buff)-1]='\0';
   strcat(buff,"--p5");
    printf("%s\n",buff);
    
    sleep(3);
  }
  return 0;
}