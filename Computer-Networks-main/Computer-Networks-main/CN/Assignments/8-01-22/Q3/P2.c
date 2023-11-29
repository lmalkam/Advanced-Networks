#include <stdio.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define SCREEN 0
#define KEYBOARD 1

// kills the process if message entered is NULL character or new line
void check_kill(char *ch, int id)
{
	if(strcmp(ch, "\n") == 0 || strcmp(ch, "\0") == 0) {
		printf("\n**NULL CHARACTER ENTERED**\n\n");
		kill(id, SIGABRT);
	}
}

int main()
{

	char *msg1, *msg2;	
	pid_t sc=20, ky=21;
	size_t size = BUFFER_SIZE;

    int id = getpid();
	int pid = getppid();

	msg1 = (char *) malloc (size);
	msg2 = (char *) malloc (size);

	int read_fd,write_fd;
    read_fd = dup(SCREEN);
    write_fd = dup(KEYBOARD);


	int pd = fork();
	
	if(pd > 0)
	{
		while(1)
		{
			dup2(ky , KEYBOARD);
			printf("I am Process 2\n");
			memset(msg1, 0, sizeof(msg2));
			read(read_fd, msg1, BUFFER_SIZE);
			check_kill(msg1,pid);
			write(read_fd, msg1, BUFFER_SIZE);
			
			
			memset(msg1, 0, sizeof(msg2));
			read(write_fd, msg1, BUFFER_SIZE);
			check_kill(msg1,pid);
			write(write_fd, msg1, BUFFER_SIZE);
		}	
	}
	else
	{
		char *args[] = {"./P3",NULL};
		execv(args[0],args);
		
	}
	
	
	
}
