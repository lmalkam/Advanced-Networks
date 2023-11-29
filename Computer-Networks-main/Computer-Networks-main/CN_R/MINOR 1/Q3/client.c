#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>
#include <poll.h>

//portno should be known beforehand
#define h_addr h_addr_list[0]
int PORT[3]={55551,55552,55553};
struct msg {
    char u_name[100]; //username
    char data[100]; //message content
};

int sfd;
char *name = "localhost"; //name can be ipv4 address(leave it as localhost if local chatting)
struct sockaddr_in serv_addr, cli_addr;//server_details address
struct hostent *server_details;//server_details details
struct msg buff, buff1;

int myConnect(int port)
{
      //gets server details by name of the server
  server_details = gethostbyname(name);
  if(server_details == NULL) {
      perror("Invalid host name");
      exit(1);
  }

  if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      exit(1);
  }

  //initialize serv_addr
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT[port-1]);//htons adjusts byte ordering

  //copy server address from server_details
  bcopy((char *) server_details->h_addr,(char *) &serv_addr.sin_addr.s_addr,server_details->h_length);

    if(connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        return -1;
    }
}
int main()
{
    printf("press 1 for PORT1\n press 2 for PORT2\n press 3 for PORT3\n");
    int port;
    scanf("%d",&port);
    myConnect(port);
    struct pollfd fds[2];
    fds[0].fd=stdin;
    fds[0].events=POLLIN;
    fds[1].fd=sfd;
    fds[1].events=POLLIN;
    while(1)
    {
        int ret=poll(fds,2,-1);
        if(ret==-1)
            continue;
        if(fds[0].revents&POLLIN)
        {
            char buff[100];
            memset(buff,0,sizeof(buff));
            fgets(buff,100,stdin);
            write(sfd,buff,sizeof(buff));
        }    
        if(fds[1].revents&POLLIN)
        {
            char buff[100];
            memset(buff,0,sizeof(buff));
            read(sfd,buff,sizeof(buff));
            printf("%s\n",buff);
        }
    }
}