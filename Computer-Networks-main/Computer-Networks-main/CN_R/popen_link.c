#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

int main(int argv,char *args[])
{
	int p1[2];
	pipe(p1);
	close(p1[0]);
	printf("enter file name\n");
	char buff[100];
	scanf("%s",buff);
	int fd=dup(0);
	dup2(p1[0],0);
	write(p1[1],buff,sizeof(buff));
	close(p1[1]);
	
	
	FILE *file = popen("./p3","r");
	dup2(fd,0);
	char c;
	while((c=fgetc(file))!=EOF)
	{
		putchar(c);
			
	}
	pclose(file);
	printf("\n");
	fflush(stdout);
	return 0;

}
		
		
