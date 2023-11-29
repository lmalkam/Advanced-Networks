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
#define PORT 9090
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
  struct sockaddr_in server_addr,client_addr;
  socklen_t addr_len;
  int opt=1;

  if((sfd=socket(AF_INET,SOCK_DGRAM,0))==-1)
  {
    perror("Error while creating socket");
    exit(0);
  }

  server_addr.sin_family=AF_INET;
  server_addr.sin_port=htons(PORT);
  server_addr.sin_addr.s_addr=INADDR_ANY;
  bzero((void *)(&(server_addr.sin_zero)),8);

  addr_len=sizeof(server_addr);

  if((bind(sfd,(struct sockaddr*)(&server_addr),addr_len))==-1)
  {
    perror("Error while binding the address");
    exit(0);
  }

  while (1)
  {
    struct sockaddr_in client_addr;
    socklen_t client_len;
    int bytes_recv,bytes_sent;
    char buffer_send[1024];
    char buffer_recv[1024];
    printf("waiting \n" );

    memset(&client_addr,0,sizeof(client_addr));

    bytes_recv=recvfrom(sfd, (char *)buffer_recv,1024,MSG_WAITALL, ( struct sockaddr *) &client_addr,&client_len);

    if(strcmp(buffer_recv,QUIT)==0)
    {
      close(sfd);
      break;
    }

    buffer_recv[bytes_recv]='\0';
    printf("Received Message: %s\n",buffer_recv);

    printf("Press q to QUIT the connection\n");
    printf("Enter the data to be sent:");
    gets(buffer_send);

    if((bytes_sent=sendto(sfd, (const char *)buffer_send, strlen(buffer_send),MSG_CONFIRM, (const struct sockaddr *) &client_addr,client_len))==-1)
    {
      perror("Send has failed");
      exit(0);
    }

    if(strcmp(buffer_send,QUIT)==0)
    {
      close(sfd);
      break;
    }

  }

}
