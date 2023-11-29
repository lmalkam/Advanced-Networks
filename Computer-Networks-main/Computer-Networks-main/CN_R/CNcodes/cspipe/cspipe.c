#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

int main()
{
 char s[100];
 int read1=dup(0);
 int writ=dup(1);
 int pp[2];
 pipe(pp);
 fgets(s,sizeof(s),stdin);
 dup2(pp[0],0); 
 dup2(pp[1],1);
 printf("%s",s);
 FILE *f1=popen("./a","r"); 
 dup2(fileno(f1),0);
 FILE *f2=popen("./b","r");
 dup2(fileno(f2),0);
 FILE *f3=popen("./c","r");
 dup2(fileno(f3),0);
 char buf[100];
 read(1,buf,100);
 dup2(writ,1);
 printf("%s",buf);
}
