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
	int fd=open(buff,O_RDONLY);
	while(read(fd, buff, 199))
		printf("%s ",buff);
}
