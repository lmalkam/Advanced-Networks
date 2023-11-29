#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#define PORT 8080
#define QUIT "q"

int sfd=0;

void sigint(int sig)
{
    close(sfd);
    exit(0);
}

int main()
{
  signal(SIGINT,sigint);
  struct sockaddr_in server_addr;
  socklen_t addr_len;
  struct hostent *host;

  if((sfd=socket(AF_INET,SOCK_STREAM,0))==-1)
  {
    perror("Error while creating socket");
    exit(0);
  }

  host=gethostbyname("127.0.0.1");
  server_addr.sin_family=AF_INET;
  server_addr.sin_port=htons(PORT);
  server_addr.sin_addr=*(struct in_addr*)host->h_addr;
  bzero((void *)(&(server_addr.sin_zero)),8);

  addr_len=sizeof(server_addr);
  if(connect(sfd,(struct sockaddr*)(&server_addr),addr_len)==-1)
  {
    perror("Error while connecting");
    exit(0);
  }
  while (1)
  {
    int bytes_recv,bytes_sent;
    char buffer_send[1024];
    char buffer_recv[1024];

    printf("Press q to QUIT the connection\n");
    printf("Enter the data to be sent:");
    gets(buffer_send);
    if(strcmp(buffer_send,QUIT)==0)
    {
      close(sfd);
      break;
    }
    if((bytes_sent=send(sfd,&buffer_send,strlen(buffer_send),0))==-1)
    {
      perror("Send has failed");
      exit(0);
    }

    bytes_recv=recv(sfd,&buffer_recv,1024,0);
    if(strcmp(buffer_recv,QUIT)==0)
    {
      close(sfd);
      break;
    }
    buffer_recv[bytes_recv]='\0';
    printf("Received Message: %s\n",buffer_recv);
  }


}
