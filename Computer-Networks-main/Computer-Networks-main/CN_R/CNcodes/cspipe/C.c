#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

int main()
{
 char s[100];
 fgets(s, sizeof(s), stdin);
 printf("%s ",s);
}
