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

  int opt=1;
  if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
      perror("socket failed");
      exit(EXIT_FAILURE);
  }
  printf("enter the server you want to join 8080 or 8081\n");
  int por;
  scanf("%d",&por);
  c_ad.sin_family = AF_INET;
  c_ad.sin_port = htons(por);
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
/*  struct pollfd fds[2];
  fds[0].events=fds[1].events=POLLIN;
  fds[0].fd=sfd;
  fds[1].fd=0;*/
  while(1)
  {
    char inp[1000],fin[1000];
    inp[0]='\0';
    fgets(inp,1000,stdin);
    inp[strlen(inp)-1]='\0';
    write(sfd,inp,strlen(inp)+1);
    read(sfd,inp,strlen(inp)+1);
    printf("%s\n",inp);
    /*
    int ret=poll(fds,2,500);
    if(!ret)  continue;
    if(fds[0].revents&POLLIN)
    {
      char out[100];
      read(fds[0].fd,out,100);
      printf("%s\n",out);
    }
    if(fds[1].revents&POLLIN)
    {
      char inp[1000],fin[1000];
      inp[0]='\0';
      fgets(inp,1000,stdin);
      inp[strlen(inp)-1]='\0';
      write(sfd,inp,strlen(inp)+1);
    }*/
  }
  return 0;
}
