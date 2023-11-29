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

#define PORT1 55551
#define PORT2 55552
#define PORT3 55553
struct sockaddr_in s_ad1,s_ad2,s_ad3,c_ad,c_list[100];
int sfd1,sfd2,sfd3,nsfd[100];
int flag1=0,flag2=0,flag3=0;

int main()
{
    //SOCKET1
  memset(&s_ad1, 0, sizeof(s_ad1));
  s_ad1.sin_addr.s_addr = INADDR_ANY;
  s_ad1.sin_family = AF_INET;
  s_ad1.sin_port = htons(PORT1);

  if((sfd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      exit(1);
  }

  //setsockopt to reuse the same address
  int opt = 1;
  if((setsockopt(sfd1, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) < 0) {
      perror("setsockopt");
      exit(1);
  }

  //bind address to socket fd
  if(bind(sfd1, (struct sockaddr*)&s_ad1, sizeof(s_ad1)) < 0) {
      perror("bind");
      exit(1);
  }

  if(listen(sfd1, 5) < 0) {
      perror("listen");
      exit(1);
  }

    //SOCKET2
  memset(&s_ad2, 0, sizeof(s_ad2));
  s_ad2.sin_addr.s_addr = INADDR_ANY;
  s_ad2.sin_family = AF_INET;
  s_ad2.sin_port = htons(PORT1);

  if((sfd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      exit(1);
  }

  opt = 1;
  if((setsockopt(sfd2, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) < 0) {
      perror("setsockopt");
      exit(1);
  }


  if(bind(sfd2, (struct sockaddr*)&s_ad2, sizeof(s_ad2)) < 0) {
      perror("bind");
      exit(1);
  }

  if(listen(sfd2, 5) < 0) {
      perror("listen");
      exit(1);
  }

    //SOCKET3
  memset(&s_ad3, 0, sizeof(s_ad3));
  s_ad3.sin_addr.s_addr = INADDR_ANY;
  s_ad3.sin_family = AF_INET;
  s_ad3.sin_port = htons(PORT1);

  if((sfd3 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      exit(1);
  }

  opt = 1;
  if((setsockopt(sfd3, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) < 0) {
      perror("setsockopt");
      exit(1);
  }

  if(bind(sfd3, (struct sockaddr*)&s_ad3, sizeof(s_ad3)) < 0) {
      perror("bind");
      exit(1);
  }

  if(listen(sfd3, 5) < 0) {
      perror("listen");
      exit(1);
  }

    struct pollfd fds[3];
    fds[0].events=POLLIN;
    fds[1].events=POLLIN;
    fds[2].events=POLLIN;
    while(1)
    {
        int ret=poll(fds,3,-1);
        if(ret==-1)
            continue;
        if(fds[0].revents&POLLIN)
        {
            if(flag1==0)
            {
                flag1=1;
                int c=fork();
                if(c==0)
                {
                    dup2(fds[0].fd,0);
                    char *args[]={NULL};
                    if(execvp("./s1",args)==-1)
                        perror("exec s1\n");
                }
            }
        } 

        if(fds[1].revents&POLLIN)
        {
            if(flag2==0)
            {
                flag2=1;
                int c=fork();
                if(c==0)
                {
                    dup2(fds[1].fd,0);
                    char *args[]={NULL};
                    if(execvp("./s2",args)==-1)
                        perror("exec s2\n");
                }
            }
        }

        if(fds[2].revents&POLLIN)
        {
            if(flag3==0)
            {
                flag3=1;
                int c=fork();
                if(c==0)
                {
                    dup2(fds[0].fd,0);
                    char *args[]={NULL};
                    if(execvp("./s3",args)==-1)
                        perror("exec s3\n");
                }
            }
        }   
    }

}