#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
int main()
{
    int fd=open("/dev/calculator",O_RDWR);
    if(fd<0)
    {
        printf("Failed to open device driver\n");
        return 0;
    }
    printf("Calculator Device Driver Opened Successfully\n");
    while(1)
    {
        printf("Enter Expression : ");
        char s[1000];
        fgets(s,1000,stdin);
        if(s[0]=='\n'&&strlen(s)<2)
            break;
        write(fd,s,strlen(s));
        char s1[1000];
        int r=read(fd,s1,1000);
        s1[r]='\0';
        printf("Response : %s\n",s1);
    }
    return 0;
}
