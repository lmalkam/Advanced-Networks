#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
int main(int argv,char *args[])
{
	int p1[2],p2[2];
	pipe(p1);
	pipe(p2);
	int c=fork();
	if(c>0)
	{
		close(p1[0]);
		close(p2[1]);
		char buff[100];
		printf("enter fine name\n");
		scanf("%s",buff);
		write(p1[1],buff,sizeof(buff));		
		close(p1[1]);	
		while(1)
		{
			if(read(p2[0],buff,sizeof(buff))>0)
				printf("%s ",buff);
			else
				break;
		}
		fflush(stdout);
	}
	else
	{
		close(p1[1]);
		close(p2[0]);
		dup2(p1[0],0);
		dup2(p2[1],1);
		args[0] = NULL;
		execvp("./p2",args);
	}
}
