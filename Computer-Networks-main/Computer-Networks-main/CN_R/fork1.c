#include<stdio.h>
#include<unistd.h>

int main()
{
	int c=fork();
	if(c>0)
	{
		printf("1\n");
		//wait(NULL);
		printf("2\n");
	}
	else
	{
		printf("3\n");
		char *args[]={"./p2",NULL};
		execvp(args[0],args);
		printf("5\n");
	}
	return 0;
}

