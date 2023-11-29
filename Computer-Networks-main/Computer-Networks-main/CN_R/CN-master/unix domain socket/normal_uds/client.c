#include "all_socket_functions.h"
#define ADDRESS "socket"

int usfd,nsfd;
void sigint(int sig)
{
  // handle_tcp_server(usfd);
  // char buffer[1024];
  // sprintf(buffer,"%d",1);
  // send(usfd,&buffer,sizeof(buffer),0);
   send_fd(usfd,nsfd);
   close(usfd);
   close(nsfd);
   exit(0);

}
int main()
{
  signal(SIGINT,sigint);
  usfd=create_uds_client(ADDRESS);
  char buffer[1024];
  gets(buffer);
  send(usfd,&buffer,sizeof(buffer),0);
  nsfd=recv_fd(usfd);
  handle_tcp_client(nsfd);
}
