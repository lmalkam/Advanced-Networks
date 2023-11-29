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

#define PORT 55555
struct mes{
  char name[100];
  char data[100];
}buf;
struct sockaddr_in s_ad,c_ad,c_list[100];
int sfd,nsfd[100];
int used=0,connected[100]={0};
fd_set rfd;
int echo_id[100]={0};
void hdfn2(int signo)
{
  for(int i=0;i<used;i++)
  {
    if(connected[i])
    {
      int c=fork();
      if(c==0)
      {
        dup2(nsfd[i],1);
        char *args[]={NULL};
        if(execvp("./echo",args)==-1)
          perror("exec echo\n");
      }

      connected[i]=0;
      FD_CLR(nsfd[i],&rfd);
      echo_id[i]=c;
      break;
    }
  }
}

void hdfn3(int signo,siginfo_t *siginfo,void* context)
{
  long s_pid=siginfo->si_pid;
  for(int i=0;i<used;i++)
  {
    if(echo_id[i]==s_pid)
    {
      echo_id[i]=0;
      connected[i]=1;
      //no need to use rfd here i will be automatically done in while loop
      break;
    }
  }
}
int main()
{
  //signal(SIGINT,hdfn);
  signal(SIGUSR1,hdfn2);

  struct sigaction act;
  memset(&act,'\0',sizeof(act));
  act.sa_sigaction=&hdfn3;
  act.sa_flags=SA_SIGINFO;
  if(sigaction(SIGUSR2,&act,NULL)<0)
    perror("sigaction");

  int c=fork();
  if(c==0)
  {
    char *args[]={NULL};
    if(execvp("./p4",args)==-1)
      perror("exec p4\n");
  }
  memset(&s_ad, 0, sizeof(s_ad));
  s_ad.sin_addr.s_addr = INADDR_ANY;
  s_ad.sin_family = AF_INET;
  s_ad.sin_port = htons(PORT);

  if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      exit(1);
  }

  //setsockopt to reuse the same address
  int opt = 1;
  if((setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) < 0) {
      perror("setsockopt");
      exit(1);
  }

  //bind address to socket fd
  if(bind(sfd, (struct sockaddr*)&s_ad, sizeof(s_ad)) < 0) {
      perror("bind");
      exit(1);
  }

  if(listen(sfd, 5) < 0) {
      perror("listen");
      exit(1);
  }
  while(1)
  {
    int maxfd=sfd;
    FD_ZERO(&rfd);
    FD_SET(sfd,&rfd);

    for(int i=0;i<used;i++)
    {
      if(connected[i])
      {
        FD_SET(nsfd[i],&rfd);
        if(maxfd<nsfd[i])
          maxfd=nsfd[i];
      }
    }

    int ret=select(maxfd+1,&rfd,NULL,NULL,NULL);
    if(ret<0)
    {
      perror("");
      continue;
    }
    if(FD_ISSET(sfd,&rfd))
    {
      int len=sizeof(c_list[used]);
      if((nsfd[used]=accept(sfd,(struct sockaddr *)&c_list[used],&len))>0)
      {
        char buf1[100],temp[100];
        sprintf(temp, "%d", c_list[used].sin_addr.s_addr);
        inet_ntop(AF_INET, temp, buf1, sizeof(buf1));
        printf("Connected to client %s::%d\n", buf1, ntohs(c_list[used].sin_port));
        connected[used] = 1;
        used++;
      }
      else
      {
        perror("acept\n");
      }
      continue;
    }
    for(int i=0;i<used;i++)
    {
      if(connected[i]&&FD_ISSET(nsfd[i],&rfd))
      {
        int re=recv(nsfd[i],&buf,sizeof(buf),0);
        printf("%s,%s\n",buf.name,buf.data);
        if(re==0)
          connected[i]=0;
        else
        {
          for(int j=0;j<used;j++)
          {
            if(connected[j]&&j!=i)
              send(nsfd[j],&buf,sizeof(buf),0);
          }
        }
      }
    }
  }
}
