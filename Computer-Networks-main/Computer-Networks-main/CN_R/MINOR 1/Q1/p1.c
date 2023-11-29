#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<poll.h>
int main()
{
    struct pollfd fds[3];

    FILE *f1=popen("./p2","r");
    FILE *f2=popen("./p3","r");
    FILE *f3=popen("./p4","r");
    FILE *f5=popen("./p5","w");

    
    int fd5=fileno(f5);
    dup2(fd5,1);
    while(1)
    {
        fds[0].fd=fileno(f1);
    fds[0].events=POLLIN;
    fds[1].fd=fileno(f2);
    fds[1].events=POLLIN;
    fds[2].fd=fileno(f3);
    fds[2].events=POLLIN;
        int ret=poll(fds,3,-1);
        if(ret==-1){
            continue;
        }
            if(fds[0].revents&POLLIN)
            {
                dup2(fds[0].fd,0);
            
                char buff[100];
                memset(buff,0,sizeof(buff));
                read(0,&buff,sizeof(buff));
                write(1,buff,sizeof(buff));
            }
            if(fds[1].revents&POLLIN)
            {
                dup2(fds[1].fd,0);
                char buff[100];
                memset(buff,0,sizeof(buff));
                read(0,&buff,sizeof(buff));
                write(1,buff,sizeof(buff));
            }
            if(fds[2].revents&POLLIN)
            {
                dup2(fds[2].fd,0);
                char buff[100];
                memset(buff,0,sizeof(buff));
                read(0,&buff,sizeof(buff));
                write(1,buff,sizeof(buff));
            }
    }
    
}
