#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 256
#define RW_PERMISSION 0777

void readFile(int fd, int vf[], int &s)
{
   int no = 0,k;
   while(read(fd,&k,1) == 1)
   {
      if(k<48 || k>57)
      {
         vf[s++] = no;
         no = 0;
      }else{
         no = 10*no + (k-48);
      }
   }
   vf[s++] = no;
}

int main()
{
   int f1 = open("f1.txt", O_RDONLY);
   int f2 = open("f2.txt", O_RDONLY);
   int f3 = open("f3.txt", O_RDWR|O_CREAT, RW_PERMISSION);
   int s1=0,s2=0;
   int vf1[BUFFER_SIZE], vf2[BUFFER_SIZE];
   int k;

   readFile(f1, vf1, s1);
   readFile(f2, vf2, s2);
   int i=0,j=0;
   while(i<s1 && j<s2)
   {
      char numc[10];
      if(vf1[i] < vf2[j])
      {
         sprintf(numc, "%d", vf1[i++]);
         write(f3, numc, strlen(numc));
      }
      else
      {
         sprintf(numc, "%d", vf2[j++]);
         write(f3, numc, strlen(numc));
      }
      write(f3, " ",1);
   }
   while(i<s1)
   {
      char numc[10];
      sprintf(numc, "%d", vf1[i++]);
      write(f3, numc, strlen(numc));
      write(f3, " ",1);
   }
   while(j<s2)
   {
      char numc[10];
      sprintf(numc, "%d", vf2[j++]);
      write(f3, numc, strlen(numc));
      write(f3, " ",1);
   }
   close(f1);
   close(f2);
   close(f3);
}
