#include <stdio.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
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

	while(1)
	{
		read(read_fd, msg1, BUFFER_SIZE);
		check_kill(msg1,pid);
		dup2(ky , KEYBOARD);
		printf("P3.) Message \"Received\" from Process P2. [%d] to Process P3 [%d]: %s\n", pid, id, msg1);
		
		memset(msg2, 0, sizeof(msg2));
		printf("P3.) \"Send\" Message from Process P3 [%d]: ",id);
		dup2(sc, SCREEN);
		getline(&msg2, &size, stdin);
		write(write_fd, msg2, BUFFER_SIZE);
	}

	close(read_fd);
	close(write_fd);
	
}
