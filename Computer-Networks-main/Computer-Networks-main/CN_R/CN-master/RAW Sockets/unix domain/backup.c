#include "all_socket_functions.h"
#define ADDRESS "ADDRESS"
#define PORT 8080
int sfd,usfd;
int fd_array[10];



int main()
{
  usfd=create_uds_client(ADDRESS);
  sfd=recv_fd(usfd);
  printf("%d\n",sfd);
  handle_tcp_client(sfd);


}
