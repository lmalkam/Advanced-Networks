#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#define MSGSIZE 30

char *msg1="Any One hearing this message";

int main()
{
	int pp[2];
	char *args[]={"ipc_pipes_second",NULL};
	if(pipe(pp)<0)
		exit(1);
	
	pid_t pid=fork();
	
	if(pid==0)
	{
		close(pp[1]);
		dup2(pp[0],0);
		execv(args[0],args);
	}
	else 
	{
		close(pp[0]);
		write(pp[1],msg1,MSGSIZE);
	}
	
	return 0;
}
