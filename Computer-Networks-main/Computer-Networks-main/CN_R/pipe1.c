#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
	int siz=9;
	char* msg="hello  1";
	char* msg1="hello  2";
	char* msg2="hello  3";
	int pp[2];
	int c=pipe(pp);
	if(c<0)
		exit(1);
	write(pp[1],msg,siz);
	write(pp[1],msg1,siz);
	write(pp[1],msg2,siz);
	char buffer[siz];
	read(pp[0],buffer,siz);
	printf("%s\n",buffer);
	read(pp[0],buffer,siz);
	printf("%s \n",buffer);
	read(pp[0],buffer,siz);
	printf("%s \n",buffer);
}
