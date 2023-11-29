#include "all_socket_functions.h"

int rsfd;
int opt=1;

int main()
{
  if((rsfd=socket(AF_INET,SOCK_RAW,101))<0)
  {
    perror("Error While creating the raw socket");
    exit(0);
  }

  struct sockaddr_in client_addr,server_addr;
  socklen_t client_len,server_len;

  client_addr.sin_family=AF_INET;
  client_addr.sin_addr.s_addr=inet_addr("127.0.0.2");
  client_len=sizeof(client_addr);

  if(bind(rsfd,(struct sockaddr*)&client_addr,client_len)<0)
  {
    perror("Error while binding");
    exit(0);
  }


  while (1)
  {
    char buffer[4096];
    memset(buffer,0,4096);
    server_len=sizeof(server_addr);
    recvfrom(rsfd,buffer,4096,0,(struct sockaddr*)&server_addr,&server_len);
    struct iphdr *ip;
	  ip=(struct iphdr*)buffer;
		printf("%s\n",(buffer+(ip->ihl)*4));
  }

}
