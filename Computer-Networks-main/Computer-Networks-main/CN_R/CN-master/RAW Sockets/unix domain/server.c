#include "all_socket_functions.h"
#define ADDRESS "ADDRESS"
#define PORT 8080
int sfd,usfd;
int fd_array[10];
int nsfd,nusfd;

void sigint(int sig)
{
  printf("sending the fd %d\n",nsfd );
  usfd=create_uds_server(ADDRESS);
  nusfd=accept_uds_client(usfd);
  send_fd(nusfd,nsfd);
}

int main()
{
  signal(SIGINT,sigint);
  sfd=create_tcp_server(PORT);
  nsfd=accept_tcp_clients(sfd);
  handle_tcp_client(nsfd);

}
