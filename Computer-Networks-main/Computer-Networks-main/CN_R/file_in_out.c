#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
int main()
{
	int p1[2],p2[2];
	int c1=pipe(p1);
	int c2=pipe(p2);
	int c3=fork();
	if(c3>0)
	{
		close(p1[0]);
		close(p2[1]);
		char args[100];
		printf("this is file name\n");
		scanf("%s",args);
		write(p1[1],args,sizeof(args));
		close(p1[1]);
		wait(NULL);
		while(1)
		{
			if(read(p2[0],args,sizeof(args))>0)
				printf("%s ",args);
			else
				break;
		}

		fflush(stdout);
	}
	else
	{
		printf("wkf\n");
		close(p1[1]);
		close(p2[0]);
		char buffer[100];
		read(p1[0],buffer,sizeof(buffer));
		//int fptr=open(buffer,RDONLY);
		printf("wkebfkwjf\n");
		FILE* fd=fopen(buffer,"r");
		//char ch=fgetc(fd);
		while(fscanf(fd,"%s",buffer)!=EOF)
		{
			
			write(p2[1],buffer,sizeof(buffer));
			
		}
		close(p2[1]);
		fclose(fd);
	}
	return 0;
}
		
