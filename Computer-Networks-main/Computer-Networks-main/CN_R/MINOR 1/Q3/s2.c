#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <poll.h>
#include <errno.h>

#define PORT1 55552

struct sockaddr_in s_ad,c_ad,c_list[100];
int sfd,nsfd[100];
int used=0;
int connected[10]={0};
int main()
{
   dup2(0,sfd);

   struct pollfd fds[6];
   fds[0].fd=sfd;
   fds[0].events=POLLIN;
   fds[0].revents=0;

   while(1)
   {
       int ret=poll(fds,used+1,-1);
       if(ret==-1)
            continue;
       if(fds[0].revents&POLLIN)
       {
           int len=sizeof(c_list[used]);
           if((nsfd[used]=accept(sfd,(struct sockaddr *)&c_list[used],&len))>0)
           {
               char buff[100];
               memset(buff,0,sizeof(buff));
               strcpy(buff,"hello!!Newbie");
               printf("new client joined to me \n");
               write(nsfd[used],buff,sizeof(buff));
               char buf1[100],temp[100];
               sprintf(temp, "%d", c_list[used].sin_addr.s_addr);
               inet_ntop(AF_INET, temp, buf1, sizeof(buf1));
               printf("Connected to client %s::%d\n", buf1, ntohs(c_list[used].sin_port));
               connected[used] = 1;
               fds[used+1].fd=nsfd[used];
               fds[used+1].events=POLLIN;
               fds[used+1].revents=0;
               used++;
           }
           continue;
       }  
        for(int i=0;i<=used;i++)
        {
            if(fds[i].revents&POLLIN)
            {
                char buff[100];
                memset(buff,0,sizeof(buff));
                read(nsfd[i],buff,sizeof(buff));
                char msg[100];
                memset(msg,0,sizeof(msg));
                for(int i=0;i<strlen(buff);i++)
                {
                    msg[i]=toupper(buff[i]);
                }
                write(nsfd[i],msg,sizeof(msg));
            }
        }
   } 
}