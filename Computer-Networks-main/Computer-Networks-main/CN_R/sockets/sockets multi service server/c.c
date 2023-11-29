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
#include <arpa/inet.h>

#define PORT 8080

struct msg_str{
  char name[100];
  char data[100];
}msg;

int sfd;
struct sockaddr_in c_ad;

int main()
{
  char name[100];
  printf("enter name:\n");
  fgets(name,100,stdin);
  name[strlen(name)-1]='\0';
  char conf[]="Linked";
  int opt=1;
  if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
      perror("socket failed");
      exit(EXIT_FAILURE);
  }
  c_ad.sin_family = AF_INET;
  c_ad.sin_port = htons(PORT);
  if (inet_pton(AF_INET, "127.0.0.1", &c_ad.sin_addr) <= 0)
  {
      printf("\nInvalid address or Address not supported \n");
      return -1;
  }
  if (connect(sfd, (struct sockaddr *)&c_ad, sizeof(c_ad)) < 0)
  {
      printf("\nConnection Failed \n");
      return -1;
  }
  read(sfd,&msg,sizeof(msg));
  if(strcmp(msg.data,conf))
  {
    printf("can't connect try again\n");
    return 0;
  }
  struct pollfd fds[2];
  fds[0].events=fds[1].events=POLLIN;
  fds[0].fd=sfd;
  fds[1].fd=0;
  while(1)
  {
    int ret=poll(fds,2,500);
    if(!ret)  continue;
    if(fds[0].revents&POLLIN)
    {
      read(fds[0].fd,&msg,sizeof(msg));
      printf("%s:%s\n",msg.name,msg.data);
    }
    if(fds[1].revents&POLLIN)
    {
      char inp[1000],fin[1000];
      inp[0]='\0';
      fgets(inp,1000,stdin);
      inp[strlen(inp)-1]='\0';
      strcpy(msg.name,name);
      strcpy(msg.data,inp);
      write(sfd,&msg,sizeof(msg));
    }
  }
  return 0;
}
