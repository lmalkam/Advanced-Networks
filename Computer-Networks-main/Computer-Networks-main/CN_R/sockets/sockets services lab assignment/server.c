#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT1 8080
#define PORT2 8081
int sfd1,sfd2;
struct sockaddr_in s1_ad,s2_ad,c_ad;
int c_len;
int nsfd;
int main()
{
  int opt=1;
  if ((sfd1 = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
      perror("socket failed");
      exit(EXIT_FAILURE);
  }
  if (setsockopt(sfd1, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
      perror("setsockopt");
      exit(EXIT_FAILURE);
  }
  s1_ad.sin_family = AF_INET;
  s1_ad.sin_addr.s_addr = INADDR_ANY;
  s1_ad.sin_port = htons(PORT1);
  if (bind(sfd1, (struct sockaddr *)&s1_ad, sizeof(s1_ad)) < 0)
  {
      perror("bind failed");
      exit(EXIT_FAILURE);
  }
  if (listen(sfd1, 3) < 0)
  {
      perror("listen");
      exit(EXIT_FAILURE);
  }
  opt=1;
  if ((sfd2 = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
      perror("socket failed");
      exit(EXIT_FAILURE);
  }
  if (setsockopt(sfd2, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
      perror("setsockopt");
      exit(EXIT_FAILURE);
  }
  s2_ad.sin_family = AF_INET;
  s2_ad.sin_addr.s_addr = INADDR_ANY;
  s2_ad.sin_port = htons(PORT2);
  if (bind(sfd2, (struct sockaddr *)&s2_ad, sizeof(s2_ad)) < 0)
  {
      perror("bind failed");
      exit(EXIT_FAILURE);
  }
  if (listen(sfd2, 3) < 0)
  {
      perror("listen");
      exit(EXIT_FAILURE);
  }
  printf("Server started\n");
  struct pollfd fds[2];
  fds[0].fd=sfd1;
  fds[1].fd=sfd2;
  fds[0].events=fds[1].events=POLLIN;
  while(1)
  {
    int ret=poll(fds,2,-1);
    if(!ret)  continue;
    if(fds[0].revents&POLLIN)
    {
      if((nsfd=accept(sfd1,(struct sockaddr *)&c_ad,(socklen_t*)&c_len))<0)
      {
        perror("accepted");
        exit(EXIT_FAILURE);
      }

      int c=fork();
      if(c==0)
      {
        //close(sfd1);
        dup2(nsfd,0);
        dup2(nsfd,1);
        char *args[]={"./s1",NULL};
        execvp(args[0],args);

      }
      else
      {
        close(nsfd);
      }

    }
    if(fds[1].revents&POLLIN)
    {
      if((nsfd=accept(sfd2,(struct sockaddr *)&c_ad,(socklen_t*)&c_len))<0)
      {
        perror("accepted");
        exit(EXIT_FAILURE);
      }

      int c=fork();
      if(c==0)
      {
      //  close(sfd2);
        dup2(nsfd,0);
        dup2(nsfd,1);
        char *args[]={"./s2",NULL};
        execvp(args[0],args);

      }
      else
      {
        close(nsfd);
      }
    }
  }

}
