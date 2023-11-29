#include "all_socket_functions.h"

int rsfd,sfd;

void sigint(int sig)
{
  close(rsfd);
  close(sfd);
  exit(0);
}

int main(int argc,char** argv)
{
  if(argc<2)
  {
    perror("The ip address is missing");
    exit(0);
  }
  signal(SIGINT,sigint);
  rsfd=create_raw_socket(argv[1]);
  char buffer[1024];
  memset(buffer,0,1024);

  struct sockaddr_in server_addr;
  socklen_t server_len;

  server_addr.sin_family=AF_INET;
  server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
  server_len=sizeof(server_addr);
  printf("Enter the query::");
  fgets(buffer,1024,stdin);

  if(sendto(rsfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&server_addr,server_len)<0)
    perror("Error in send");

  while(1)
  {
    memset(buffer,0,1024);
    recvfrom(rsfd,buffer,4096,0,(struct sockaddr*)&server_addr,&server_len);
    struct iphdr *ip;
	  ip=(struct iphdr*)buffer;
		printf("%s\n",(buffer+(ip->ihl)*4));
    int port_no=atoi((buffer+(ip->ihl)*4));
    sfd=create_tcp_client(port_no);
    pid_t pid=fork();
    if(pid==0)
      handle_tcp_server(sfd);
    else
      close(sfd);
  }

}
