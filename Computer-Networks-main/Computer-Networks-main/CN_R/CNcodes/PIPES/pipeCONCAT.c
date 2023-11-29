#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

int main()
{
 char fxd_str[]="Bansal";
 int pp1[2],pp2[2];
 pipe(pp1);
 pipe(pp2);
 int c=0;
 char buf1[100],final_str[100];
 c=fork();
 if(c>0)
 {
   close(pp1[0]);
   close(pp2[1]);
   char *input_str;
   scanf("%s",input_str);
   write(pp1[1],input_str,strlen(input_str)+1);
   //close(pp1[1]);
   wait(NULL);
   read(pp2[0],final_str,100);
   printf("%s",final_str);
   //close(pp2[0]);
 }
 else
 {
   close(pp1[1]);
   close(pp2[0]);
   read(pp1[0],buf1,100);
   //close(pp1[0]);
   write(pp2[1],buf1,strlen(buf1)+1);
   //close(pp2[1]);
   exit(0);
 }
}
