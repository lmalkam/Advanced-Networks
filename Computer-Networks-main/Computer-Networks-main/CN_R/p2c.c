#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
	int p[2];
	int k=pipe(p);
	int c=fork();
	
	if(c>0)
	{
		close(p[0]);
		write(p[1],"hello",10);	
	}
	else
	{
		close(p[1]);
		char buffer[10];
		read(p[0],buffer,10);
		printf("%s\n",buffer);
	}
	return 0;
}
