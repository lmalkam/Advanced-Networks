#include "all_socket_functions.h"
#define ADDRESS "socket"

int usfd,sfd;
int sfd_count=2;
void sigint(int sig)
{
  close(usfd);
  close(sfd);
  exit(0);
}
int fds[2];
int main()
{


   signal(SIGINT,sigint);
  printf("Welcome to the server\n");
  usfd=create_uds_server(ADDRESS);
  int nusfd=accept_uds_client(usfd);
  // fflush(stdout);
   printf("USD client accepted" );
  sfd=create_tcp_server(5050);
  int nsfd=accept_tcp_clients(sfd);
  printf("TCP client accepted" );
  int pid=fork();
  if(pid==0)
  {
    char buffer[1024];
    recv(nusfd,&buffer,1024,0);
    printf("%s\n",buffer);
    send_fd(nusfd,nsfd);
    close(nsfd);
    nsfd=recv_fd(nusfd);
    handle_tcp_client(nsfd);
    }



}
