#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<unistd.h>
int main()
{
	mkfifo("red",0666);
	char buff[100];
	int fd=open("red",O_WRONLY);

	while(1)
	{
		scanf("%s",buff);
		write(fd,buff,sizeof(buff));
	}
}
