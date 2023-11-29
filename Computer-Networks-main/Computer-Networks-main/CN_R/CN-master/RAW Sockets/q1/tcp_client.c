#include "all_socket_functions.h"
#define PORT 5050
int sfd;
int main()
{
  sfd=create_tcp_client(PORT);
  char buffer_send[1024];
  memset(buffer_send,0,1024);
  fgets(buffer_send,1024,stdin);

  struct sockaddr_in server_addr;
  socklen_t server_len;

  server_addr.sin_family=AF_INET;
  server_addr.sin_port=htons(PORT);
  server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

  server_len=sizeof(server_addr);

  if(send(sfd,&buffer_send,strlen(buffer_send),0)<0)
  {
    perror("Error while sending");
    exit(0);
  }
}
