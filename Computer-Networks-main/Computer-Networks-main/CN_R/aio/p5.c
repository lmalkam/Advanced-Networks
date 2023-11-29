#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main()
{
  while(true)
  {
    char m[100];
    fgets(m,100,stdin);
    printf("P5 : %s\n",m);
    fflush(stdout);
  }
  return 0;
}
