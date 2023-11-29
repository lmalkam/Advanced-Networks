#include "all_socket_functions.h"
int rsfd,sfd,opt=1;
int port_no;

void sigint(int sig)
{
  close(rsfd);
  close(sfd);
  exit(0);
}
int main(int argc,char** argv)
{
  signal(SIGINT,sigint);
  if(argc<2)
  {
    perror("argument not given");
    exit(0);
  }
  port_no=atoi(argv[1]);

  rsfd=create_raw_socket("127.0.0.1");
  sfd=create_tcp_server(port_no);

  while(1)
  {
    fd_set read_fds;
    int retval,max_fd=rsfd>sfd?rsfd:sfd;
    struct sockaddr_in client_addr;
    socklen_t client_len;
    struct timeval tv;

    // do
    // {
      retval=-1;
      FD_ZERO(&read_fds);
      tv.tv_sec=0;
      tv.tv_usec=0;
      FD_SET(rsfd,&read_fds);
      FD_SET(sfd,&read_fds);


      retval=select(max_fd+1,&read_fds,NULL,NULL,NULL);
      fflush(stdin);
      fflush(stdout);
     // }while(retval==-1);

    fflush(stdin);
    fflush(stdout);

    printf("Something has been selected");

    if(retval==-1)
    {
      perror("ERROR in select");
      return -1;
    }
    else if(FD_ISSET(rsfd,&read_fds))
    {
      char buffer[4096];
      memset(buffer,0,4096);
      client_len=sizeof(client_addr);
      recvfrom(rsfd,buffer,4096,0,(struct sockaddr*)&client_addr,&client_len);
      struct iphdr *ip;
  	  ip=(struct iphdr*)buffer;
  		printf("%s\n",(buffer+(ip->ihl)*4));
      memset(buffer,0,4096);
      sprintf(buffer,"%d",port_no);
      printf("The Port no sent is ::%s\n",buffer);
      fflush(stdin);
      fflush(stdout);
      sendto(rsfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&client_addr,client_len);
    }
    else if(FD_ISSET(sfd,&read_fds))
    {
      int nsfd=accept_tcp_clients(sfd);
      pid_t pid=fork();
      if(pid==0)
      {
        close(sfd);
        fflush(stdout);
        handle_tcp_client(nsfd);
      }
      else
        close(nsfd);
    }
  }
}
