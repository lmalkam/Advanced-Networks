#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include<ctype.h>

#define BUFFER_SIZE 256
#define RW_PERMISSION 0777

int main()
{
   int f1 = open("f1.txt", O_RDONLY);
   int f2 = open("f2.txt", O_RDWR|O_CREAT, RW_PERMISSION);
   char chr[BUFFER_SIZE];
   int i=0;

   char k,prev='-';
   int ch=0,in=1000;
   while(read(f1,&k,1) == 1)
   {
      if(k==' ')
      {
         if(prev == '-')
            continue;
         else if(isdigit(prev))
         {
            lseek(f2,in,SEEK_SET);
            write(f2,&k,1);
            in++;
         }
         else if(isalpha(prev))
         {
            lseek(f2,ch,SEEK_SET);
            write(f2,&k,1);
            ch++;
         }
      }
      else if(isdigit(k))
      {
         lseek(f2,in,SEEK_SET);
         write(f2,&k,1);
         in++;
      }
      else {
         lseek(f2,ch,SEEK_SET);
         write(f2,&k,1);
         ch++;
      }
      prev = k;
   }

   close(f1);
   close(f2);
}