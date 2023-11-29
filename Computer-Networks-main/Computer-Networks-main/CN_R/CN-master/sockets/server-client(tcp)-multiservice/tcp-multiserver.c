#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
#define services 2
#define PORT 5100
#define BKLOG 5
#define QUIT "q"

int max_sfd=-1;
int ports[services];
int sfd[services];

char serv_exe[services][10];

void sigint(int sig)
{
  for(int i=0;i<services;i++)
    close(sfd[i]);
  exit(0);
}

void init_serv_exe()
{
  char str[5];
  strcpy(str,"./s");
  for(int i=0;i<services;i++)
  {
    char temp[5];
    sprintf(temp,"%d",i+1);
    strcpy(serv_exe[i],str);
    strcat(serv_exe[i],temp);
  }
}

void init_socks()
{
  // printf("Initilise Sockets\n");
  struct sockaddr_in server_addr;
  socklen_t addr_len;
  int opt=1;

  server_addr.sin_family=AF_INET;
  server_addr.sin_addr.s_addr=INADDR_ANY;
  bzero((void *)(&server_addr.sin_zero),8);

  for(int i=0;i<services;i++)
  {
    if((sfd[i]=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
      perror("ERROR while creating a socket");
      exit(EXIT_FAILURE);
    }

    max_sfd=max_sfd>sfd[i]?max_sfd:sfd[i];
    // printf("SFDs%d %d\n",sfd[i],max_sfd );

    if (setsockopt(sfd[i], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_port=htons(PORT+i*100);
    addr_len=sizeof(server_addr);

    if(bind(sfd[i],(struct sockaddr*)(&server_addr),addr_len)==-1)
    {
      perror("ERROR while binding");
      exit(EXIT_FAILURE);
    }

    if(listen(sfd[i],BKLOG)==-1)
    {
      perror("ERROR while listening");
      exit(EXIT_FAILURE);
    }
  }
}

void handle_client(int nsfd,int ind)
{
  // printf("Handle Client\n");
  char temp_nsfd[5];
  sprintf(temp_nsfd,"%d",nsfd);
  char *args[]={serv_exe[ind],temp_nsfd,NULL};
  execv(args[0],args);
}

void select_clients()
{
  // printf("Select Clients\n");
  fd_set read_fds;
  int retval,nsfd,pid;
  socklen_t addr_len;
  struct sockaddr_in client_addr;

  struct timeval tv;

  while(1)
  {
    retval=-1;
    do
    {
      FD_ZERO(&read_fds);
      tv.tv_sec=5;
      tv.tv_usec=0;
      for(int i=0;i<services;i++)
        FD_SET(sfd[i],&read_fds);
      retval=select(max_sfd+1,&read_fds,NULL,NULL,&tv);
    }while(retval==-1);

    if(retval==-1)
    {
      perror("ERROR in select");
      exit(EXIT_FAILURE);
    }
    else if(retval>0)
    {
      for(int i=0;i<services;i++)
      {
        if(FD_ISSET(sfd[i],&read_fds))
        {
          addr_len=sizeof(struct sockaddr_in);
          if((nsfd=accept(sfd[i],(struct sockaddr*)(&client_addr),&addr_len))==-1)
          {
            perror("ERROR in accepting the client");
            exit(EXIT_FAILURE);
          }

          pid=fork();
          if(pid<0)
          {
            perror("ERROR while forking");
            exit(EXIT_FAILURE);
          }
          else if(pid==0)
          {
            close(sfd[i]);
            handle_client(nsfd,i);
          }
          else
            close(nsfd);
        }
      }
    }
    // else if(retval==0)
    //   printf("Keep waiting for the clients\n");
  }
}

int main()
{
  signal(SIGINT,sigint);
  init_serv_exe();
  init_socks();
  select_clients();
}
