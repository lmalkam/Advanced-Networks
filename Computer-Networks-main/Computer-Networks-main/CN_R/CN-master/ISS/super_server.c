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
#define QUIT "q"
#define BKLOG 10
#define MAX_FD -1

struct init_type{
  char service[10];
  int port;
  char protocol[10];
  int limit;
  char in_ex[5];
  char offer_by;
};
int init_count=0;
char init_info[6][10];
struct init_type database[10];

void update_database()
{
  strcpy(database[init_count].service,init_info[0]);
  strcpy(database[init_count].protocol,init_info[1]);
  database[init_count].port=atoi(init_info[2]);
  strcpy(database[init_count].in_ex,init_info[3]);
  database[init_count].limit=atoi(init_info[4]);
  database[init_count].offer_by=init_info[5][0];
  init_count++;
}

void print_database()
{
  for(int i=0;i<init_count;i++)
  {
    printf("%s %s %d %s %d %c\n",database[i].service,database[i].protocol,database[i].port,database[i].in_ex,database[i].limit,database[i].offer_by);
  }
}

int main()
{
  FILE *fptr=fopen("init.txt","r");
  char *buffer;
  size_t bufsize = 256;
  buffer = (char *)malloc(bufsize * sizeof(char));

  while(getline(&buffer,&bufsize,fptr)!=-1)
  {
    char* token;
    char* rest = buffer;int j=0;
    while ((token = strtok_r(rest, " ", &rest)))
      strcpy(init_info[j++],token);
    update_database();
  }
  print_database();

  //allocate the space for the sfd's in order to make use them in the code sometime after
  int sfd[init_count];
  struct sockaddr_in server_addr,client_addr;
  socklen_t server_len,client_len;

  server_addr.sin_family=AF_INET;
  server_addr.sin_addr.s_addr=INADDR_ANY;
  bzero((void *)(&(server_addr.sin_zero)),8);

  for(int i=0;i<init_count;i++)
  {
    server_addr.sin_port=htons(database[i].port);

    // if the protocol is tcp
    if(strcmp(init_type[i],"tcp")==0)
    {
      if((sfd[i]=socket(AF_INET,SOCK_STREAM,0))==-1)
      {
        perror("Unabale to open a sfd");
        exit(0);
      }

      server_len=sizeof(server_addr);
      if(bind(sfd[i],(struct sockaddr*)(&server_addr),server_len))==-1)
      {
        perror("Unabale to Bind");
        exit(0);
      }
      if(listen(sfd,BKLOG)==-1)
      {
        perror("Error while listening");
        exit(0);
      }

    }
    //if the protocol is udp
    else if(strcmp(init_type[i],"udp")==0)
    {
      if((sfd[i]=socket(AF_INET,SOCK_DGRAM,0))==-1)
      {
        perror("Unabale to open a sfd");
        exit(0);
      }
    }
    // if there is any error while reading from the file
    else
    {
      printf("Error Detected int the type of protocol in init file\n");
      exit(EXIT_FAILURE);
    }
    MAX_FD=MAX_FD>sfd[i]?MAX_FD:sfd[i];
  }

  //start preparing for receiving the data from the clients
  struct fd_set read_fds;
  int ret_val=-1;
  struct timeval tv;
  do
    {
      FD_ZERO(&read_fds);
      tv.tv_sec=5;
      tv.tv_usec=0;
      for(int i=0;i<init_count;i++)
        FD_SET(sfd[i],&read_fds);
      retval=select(MAX_FD+1,&read_fds,NULL,NULL,&tv);
    }while(retval==-1);

    if(retval==-1)
    {
      perror("ERROR in select");
      exit(EXIT_FAILURE);
    }
    else if(retval>0)
    {
      for(int i=0;i<init_count;i++)
      {
        if(FD_ISSET(sfd[i],&read_fds))
        {
          //if tcp protocol
          if(strcmp(init_type[i],"tcp")==0)
          {
            client_len=sizeof(struct sockaddr_in);
            if((nsfd=accept(sfd[i],(struct sockaddr*)(&client_addr),&addr_len))==-1)
            {
              perror("ERROR in accepting the client");
              exit(EXIT_FAILURE);
            }
          }
          if()
          //if udp protocol
          else
          {

          }
        }
      }
    }
}
