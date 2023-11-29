#include "all_socket_functions.h"
#define PORT 5050
int sfd,rsfd,nsfd;
int opt=1;
int main()
{
    sfd=create_tcp_server(PORT);
    while(1)
    {
      char buffer_recv[1024];
      memset(buffer_recv,0,1024);
      printf("Im in the beginning\n" );
      nsfd=accept_tcp_clients(sfd);
      if(recv(nsfd,&buffer_recv,1024,0)<0)
      {
        perror("Error While Receiving");
        exit(0);
      }
      printf("Address Received::%s\n",buffer_recv);

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

      client_addr.sin_family=AF_INET;
      client_addr.sin_addr.s_addr=inet_addr("127.0.0.2");
      client_len=sizeof(client_addr);

      if(bind(rsfd,(struct sockaddr*)&server_addr,server_len)<0)
      {
        perror("Error while binding");
        exit(0);
      }

      if(connect(rsfd,(struct sockaddr *)&client_addr,client_len)<0)
      {
        perror("Error while Connecting");
        exit(0);
      }

      // if(sendto(rsfd,buffer_recv,strlen(buffer_recv)+1,0,(struct sockaddr*)&client_addr,client_len)<0)
      // {
      //   perror("Error While Sending data");
      //   exit(0);
      // }

      write(rsfd,buffer_recv,strlen(buffer_recv));
      printf("Forking the parent now\n" );
      pid_t pid=fork();
      if(pid==0)
      {
          printf("Hello There");
          int fd=open("file.txt",O_RDONLY);
          dup2(rsfd,1);
          dup2(fd,0);
          char* buffer[]={"./temp",NULL};
          execv("./temp",buffer);
      }

    }
}
