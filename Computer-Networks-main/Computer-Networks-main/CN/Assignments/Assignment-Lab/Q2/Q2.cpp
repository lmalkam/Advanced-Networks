#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include<ctype.h>
#define RW_PERMISSION 0777

#define BUFFER_SIZE 256


int main()
{
   int f1 = open("f1.txt", O_RDONLY);
   int f2 = open("f2.txt", O_RDWR|O_CREAT, RW_PERMISSION);
   
   int no = 0;
   char k;
   while(read(f1,&k,1) == 1)
   {
      k = toupper((char)k);
      write(f2,&k,1);
   }
   close(f1);
   close(f2);
}
