#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	printf("Im the program called by p1 using the exec commands\n");
	return 0;
}
