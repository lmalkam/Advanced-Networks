#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<unistd.h>
int main()
{
	char buff[100];
	int fd=open("red",O_RDONLY);

	while(1)
	{
		read(fd,buff,sizeof(buff)+1);
    printf("%s\n",buff);
	}
}
