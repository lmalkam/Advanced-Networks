#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
int calc(int n)
{
    return n*n;
}
int main()
{
  while(1){
  char buff[100];
memset(buff,0,sizeof(buff));
 strcpy(buff,"hello from p2");
 //buff[strlen(buff)-1]='\0';
    write(1,buff,sizeof(buff));
  sleep(3);
  }
}