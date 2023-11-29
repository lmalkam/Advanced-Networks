#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int sfd;
int nsfd[100];
struct sockaddr_in s_ad,c_ad[100];
int used=0;
int c_len[100];

struct msg_str
{
  char name[100],data[100];
}msg;
int main()
{
  int opt=1;
  if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
      perror("socket failed");
      exit(EXIT_FAILURE);
  }
  if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
      perror("setsockopt");
      exit(EXIT_FAILURE);
  }
  s_ad.sin_family = AF_INET;
  s_ad.sin_addr.s_addr = INADDR_ANY;
  s_ad.sin_port = htons(PORT);
  if (bind(sfd, (struct sockaddr *)&s_ad, sizeof(s_ad)) < 0)
  {
      perror("bind failed");
      exit(EXIT_FAILURE);
  }
  int ll=sizeof(s_ad);
  int get=getsockname(sfd,(struct sockaddr *)&s_ad,(socklen_t *) &ll);
  printf("%d\n",get);
  if (listen(sfd, 3) < 0)
  {
      perror("listen");
      exit(EXIT_FAILURE);
  }
  printf("Server started\n");
  char conf[]="Linked";
  struct pollfd fds[101];
  fds[0].fd=sfd;
  fds[0].events=POLLIN;
  while(1)
  {
    int ret=poll(fds,used+1,-1);
    if(!ret)  continue;
    for(int i=0;i<used;i++)
    {
      if(fds[i+1].revents&POLLIN) //if any input gets activated
      {
        read(nsfd[i],&msg,sizeof(msg));
        for(int j=0;j<used;j++)
        {
          if(j!=i)
            write(nsfd[j],&msg,sizeof(msg));
        }
      }
    }
    if(fds[0].revents&POLLIN)
    {
      if((nsfd[used]=accept(sfd,(struct sockaddr *)&c_ad[used],(socklen_t*)&c_len[used]))<0)
      {
        perror("accepted");
        exit(EXIT_FAILURE);
      }
      int cl= getpeername(nsfd[used], (struct sockaddr *)&c_ad[used], (socklen_t *)&c_len[used]);
      printf("%d\n",cl);
      strcpy(msg.data,conf);
      write(nsfd[used],&msg,sizeof(msg));
      used++;
      fds[used].fd=nsfd[used-1];
      fds[used].events=POLLIN;
    }
  }
  return 0;
}
