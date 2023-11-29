#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>
int main()
{
 int c;
 c=0;
 int pp[2];
 pipe(pp);
 c=fork();
 char buffer[6];
 if(c>0)
 {
   close(pp[0]);
   write(pp[1],"check",5);
 }
 else
 {
   close(pp[1]);
   read(pp[0],buffer,5);
   printf("%s",buffer);
 }
}

