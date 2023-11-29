/*In this two way communication using two different pipes*/

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#define MSGSIZE 60

char* msg1="This is the first message from parent to the child";
char* msg2="This is the second message from child to the parent";


int main()
{
	char inbuf[MSGSIZE];
	int pp1[2],pp2[2];

	if(pipe(pp1)<0)
		exit(1);
	if(pipe(pp2)<0)
		exit(1);
	
	int pid=fork();
	if(pid<0)
		exit(1);
	else if(pid==0)
	{
		close(pp1[0]);
		close(pp2[1]);
		
		write(pp1[1],msg1,MSGSIZE);
		
		read(pp2[0],inbuf,MSGSIZE);
			printf("%s\n",inbuf);

		wait(NULL);
	}
	else
	{
		close(pp2[0]);
		close(pp1[1]);
		
		write(pp2[1],msg2,MSGSIZE);
		
		read(pp1[0],inbuf,MSGSIZE);
			printf("%s\n",inbuf);
		
		printf("Finished reading the stuff\n");
	}
}
