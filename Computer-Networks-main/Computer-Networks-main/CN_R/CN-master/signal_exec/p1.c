#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	pid_t pid=fork();
	if(pid==0)
	{
		char *args[]={"p2",NULL};
		execv(args[0],args);
		
		//char *args[]={"./p2",NULL};
		//execvp(args[0],args);
		
		printf("Nothing will be printed as exec has been called\n");
	}
	else
	{
		printf("lemme use execv command to call for program p2\n");
		wait(NULL);
	}
}
