#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
int main()
{
 int x,y;
 scanf("%d %d",&x,&y);
 int t=x;
 x=y;
 y=t;
 printf("%d %d",x,y);
}
 
