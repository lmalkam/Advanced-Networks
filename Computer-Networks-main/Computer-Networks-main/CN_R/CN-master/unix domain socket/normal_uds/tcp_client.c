#include"all_socket_functions.h"
int sfd;
void sigint(int sig)
{
  close(sfd);
  exit(0);
}
int main()
{
  signal(SIGINT,sigint);
  sfd=create_tcp_client(5050);
  handle_tcp_server(sfd);
}
