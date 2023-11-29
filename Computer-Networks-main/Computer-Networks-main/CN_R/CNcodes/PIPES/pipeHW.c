#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
int main()
{
 FILE *fptr;
 char *fname;
 fname="Aspipe.txt";
 char buf[20],buf1[100];
 //size_t res = ftell(fptr); 
 int c=0;
 int pp1[2],pp2[2];
 pipe(pp1);
 pipe(pp2);
 c=fork();
 if(c>0)
 {
   close(pp1[0]);
   close(pp2[1]);
   write(pp1[1],fname,10);
   wait(NULL);
   read(pp2[0],buf1,100);
   printf("%s",buf1);
   exit(0);
 }
 else
 { 
   close(pp1[1]);
   close(pp2[0]);
   read(pp1[0],buf,10);
   fptr = fopen(buf, "r");
   char ch;
   ch = fgetc(fptr);
   while (ch != EOF)
   {
     printf ("%c", ch);
     ch = fgetc(fptr);
     char *p=ch;
     write(pp2[1],p,1);
   }
   fclose(fptr);
   exit(0);
 }
}
