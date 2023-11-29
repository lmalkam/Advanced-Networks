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

	int write_pipe[2], read_pipe[2];
	char *messsage_1, *messsage_2;
	pid_t sc=20, ky=21;
	size_t size = BUFFER_SIZE;

	// creating half duplex pipes
	pipe(write_pipe);
	pipe(read_pipe);

	// creating duplicate descriptor for screen and keyboard
	dup2(SCREEN,sc);
    dup2(KEYBOARD,ky);

    // write and read descriptor for screen and keyboard
	dup2(write_pipe[0],0);
	dup2(read_pipe[1],1);


	int pd = fork();
	
	if(pd > 0)
	{
		int id = getpid();
		close(write_pipe[0]);
		close(read_pipe[1]);
		dup2(sc,SCREEN);
        dup2(ky,KEYBOARD);
		
		// allocating message size
		messsage_1 = (char *) malloc (size);
		messsage_2 = (char *) malloc (size);
		while(1)
		{
			memset(messsage_1,0,sizeof(messsage_1));
			printf("P1.) \"Send\" Message from Parent Process [%d]: ",id);
			getline(&messsage_1, &size, stdin);
			write(write_pipe[1], messsage_1, BUFFER_SIZE);
			
			memset(messsage_2,0,sizeof(messsage_2));
			read(read_pipe[0], messsage_2, BUFFER_SIZE);
			
			// checking for null character
			check_kill(messsage_2, id);
			
			printf("P1.) Message \"Received\" in parent [%d]: %s\n\n\n", id, messsage_2);
		}
		
		close(write_pipe[1]);
		close(read_pipe[0]);	
	}
	else
	{
		char *args[] = {"./P2",NULL};
		execv(args[0],args);
		
	}
	
	
	
}
