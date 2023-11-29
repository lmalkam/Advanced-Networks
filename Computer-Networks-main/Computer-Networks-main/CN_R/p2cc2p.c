#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
	int p1[2],p2[2];
	int k=pipe(p1);
	int l=pipe(p2);
	int c=fork();
	
	if(c>0)
	{
		close(p1[0]);
		close(p2[1]);
		write(p1[1],"Xmas",10);	
		char buffer[10];
		read(p2[0],buffer,10);
		printf("%s\n",buffer);
	}
	else
	{
		close(p1[1]);
		close(p2[0]);
		write(p2[1],"merry ",10);
		char buffer[10];
		read(p1[0],buffer,10);
		printf("%s\n",buffer);
	}
	return 0;
}
