// Required header files
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
 
int main()
{
	int fd=open("/dev/mydev1",O_RDWR); // opens the device driver which has been loaded
	printf("%d\n",fd);
	char buf[100];
	int c=read(fd,buf,sizeof(100)); // reads the time of the system through device driver
	printf("%d out read %s\n",c,buf); // prints the system time in seconds(nu)
        
	memset(buf,0,256);
        scanf("%s",buf);  // takes input from user to which current time has to be set
	c=write(fd,buf,strlen(buf)); // writes the time this changes the system time
	printf("%d out write \n",c); // prints size of char buffer
        close(fd);
	return 0;
}
