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
#define QUIT "q"

int nsfd;

void sigint(int sig)
{
  close(nsfd);
  exit(0);
}

int main(int argc, char **argv)
{
  signal(SIGINT,sigint);
  printf("WELCOME TO THE SERVICE PROVIDER\n");
  if(argc<2)
  {
    perror("ERROR while passing arguments");
    exit(EXIT_FAILURE);
  }

  nsfd=atoi(argv[1]);

  while (1)
  {
    int bytes_recv,bytes_sent;
    char buffer_send[1024];
    char buffer_recv[1024];

    bytes_recv=recv(nsfd,&buffer_recv,1024,0);
    if(strcmp(buffer_recv,QUIT)==0)
    {
      close(nsfd);
      break;
    }
    buffer_recv[bytes_recv]='\0';
    printf("Received Message: %s\n",buffer_recv);

    printf("Press q to QUIT the connection\n");
    printf("Enter the data to be sent:");
    gets(buffer_send);
    if(strcmp(buffer_send,QUIT)==0)
    {
      close(nsfd);
      break;
    }
    if((bytes_sent=send(nsfd,&buffer_send,strlen(buffer_send),0))==-1)
    {
      perror("Send has failed");
      exit(0);
    }

  }
}
