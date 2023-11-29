/*In this only one sided communication is there from parent to child*/

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

#define MSGSIZE 30

char* msg1="This is the first message";
char* msg2="This is the second message";
char* msg3="This is the third message";

int main()
{
	char inbuf[MSGSIZE];
	int pp[2];
	if(pipe(pp)<0)
		exit(1);
	
	int pid=fork();
	if(pid<0)
		exit(1);
	else if(pid==0)
	{
		close(pp[0]);
		
		write(pp[1],msg1,MSGSIZE);
		write(pp[1],msg2,MSGSIZE);
		write(pp[1],msg3,MSGSIZE);
		wait(NULL);
	}
	else
	{
		close(pp[1]);
		
		while(read(pp[0],inbuf,MSGSIZE))
			printf("%s\n",inbuf);
		
		printf("Finished reading the stuff\n");
	}
}
