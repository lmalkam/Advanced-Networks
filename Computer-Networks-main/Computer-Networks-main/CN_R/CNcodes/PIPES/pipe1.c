#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
int main()
{
 int pp1[2],pp2[2];
 char c=0;
 pipe(pp1);
 pipe(pp2);
 c=fork();
 char buf1[40],buf2[40];
 if(c>0)
 {
   close(pp1[0]);
   close(pp2[1]);
   write(pp1[1],"written by parent in pp1",24);
   wait(NULL);
   read(pp2[0],buf2,24);
   printf("%s",buf2);
 }
 else
 {
   close(pp2[0]);
   close(pp1[1]);
   read(pp1[0],buf1,24);
  // wait();
   write(pp2[1],"written by parent in pp2",24);
   printf("%s",buf1);
   exit(0);
 }
}
