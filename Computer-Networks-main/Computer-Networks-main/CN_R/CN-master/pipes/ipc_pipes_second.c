#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#define MSGSIZE 30

int main()
{
	char inbuf[MSGSIZE];
	read(0,inbuf,MSGSIZE);
	printf("%s\n",inbuf);
	return 0;
}
