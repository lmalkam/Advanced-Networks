#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

int main()
{
 int rd=dup(0);
 int wr=dup(1);
 FILE *f=fopen("tc.txt","r");
 dup2(fileno(f),0);
 FILE *of1=fopen("O1.txt","w");
 dup2(fileno(of1),1);
 FILE *f1=popen("/home/shruti/Documents/CNcodes/ONLINE_JUDGE/ps","r");
dup2(fileno(f1), 0);
char s[100];
fgets(s,100,stdin);
 fflush(stdout);
 printf("%s\n",s);
// read from O1.txt
fflush(stdout);		
 fclose(of1);
fseek(f, 0, SEEK_SET);
dup2(fileno(f),0);
 FILE *of2=fopen("O2.txt","w");
 dup2(fileno(of2),1);
 FILE *f2=popen("/home/shruti/Documents/CNcodes/ONLINE_JUDGE/p","r");
 while(read(fileno(f2),s,100)>0)
 {
   printf("%s ",s);
 }
 fclose(of2);
 
}
