#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
int main()
{
 int x,y;
 scanf("%d %d",&x,&y);
 swap(&x,&y);
 printf("%d %d\n",x,y);
}
 
