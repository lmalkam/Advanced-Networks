#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	char buff[200];
	
	scanf("%s",buff);
	FILE *fd=fopen(buff,"r");
	while(fscanf(fd,"%s",buff)>0)
		printf("%s ",buff);
}
