#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
int calc(int n)
{
    return n*n;
}
int main()
{
  int n;
  scanf("%d",&n);
  n=calc(n);
  printf("%d\n",n);
}
