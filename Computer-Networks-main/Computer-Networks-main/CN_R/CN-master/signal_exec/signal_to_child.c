#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

void sighup(int sig)
{
	printf("\n Child received signal SIGHUP \n\n");
	signal(SIGHUP,sighup);

}
void sigint(int sig)
{
	printf("\n Child received signal SIGINT \n\n");
	signal(SIGINT,sigint);
}
void sigquit(int sig)
{
	printf("\n Parent Killed CHild\n\n");
	exit(0);
}

int main()
{
	int pid=fork();

	//this is the child
	if(pid==0)
	{
		signal(SIGHUP,sighup);
		signal(SIGINT,sigint);
		signal(SIGQUIT,sigquit);
		while(1);
	}
	//this is the parent section
	else
	{
		printf("\n parent sending signal SIGHUP \n\n");
		kill(pid,SIGHUP);
		sleep(3);

		printf("\n parent sending signal SIGINT \n\n");
		kill(pid,SIGINT);
		sleep(3);

		printf("\n parent sending signal SIGQUIT \n\n");
		kill(pid,SIGQUIT);
		sleep(3);
		wait(NULL);
	}
}
