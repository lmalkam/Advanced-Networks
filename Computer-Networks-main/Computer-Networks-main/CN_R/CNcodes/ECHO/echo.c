#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

int main()
{
 mkfifo("ffo",0666);
 rfd=open("ffo","r");
 wfd=open("ffo","w");
 while(1)
 {
   char s[100];
   read(rfd,s,100);
   printf("%s",s);
   fgets(s,sizeof(s),STDIN);
   write(wfd,s,100);
 }
}
