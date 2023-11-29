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

  if(setsockopt(rsfd,IPPROTO_IP,SO_BROADCAST,&opt,sizeof(int))<0)
  {
    perror("Error in set sock opt");
    exit(0);
  }

  struct sockaddr_in client_addr,server_addr;
  socklen_t client_len,server_len;

  server_addr.sin_family=AF_INET;
  server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
  server_len=sizeof(server_addr);

  if(bind(rsfd,(struct sockaddr*)&server_addr,server_len)<0)
  {
    perror("Error while binding");
    exit(0);
  }

  while (1)
  {
    char buffer[4096];
    memset(buffer,0,4096);
    client_len=sizeof(server_addr);
    recvfrom(rsfd,buffer,4096,0,(struct sockaddr*)&client_addr,&client_len);
    struct iphdr *ip;
	  ip=(struct iphdr*)buffer;
		printf("%s\n",(buffer+(ip->ihl)*4));
    printf("Enter Data to be sent" );
    memset(buffer,0,4096);
    fgets(buffer,4096,stdin);
    sendto(rsfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&client_addr,client_len);
  }

}
