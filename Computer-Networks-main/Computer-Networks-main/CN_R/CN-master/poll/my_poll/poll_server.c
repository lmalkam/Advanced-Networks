#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>

int fd[2];

void sigint(int sig)
{
  close(fd[0]);
  close(fd[1]);
  exit(0);
}

int main()
{
    struct pollfd pfd[2];
    signal(SIGINT,sigint);

    FILE* fl1=popen("./client1","r");
    FILE* fl2=popen("./client2","r");



    fd[0]=fileno(fl1);
    fd[1]=fileno(fl2);

    for(int i=0;i<2;i++)
    {
      pfd[i].fd=fd[i];
      pfd[i].events=POLLIN;
    }
    while (1)
    {
      int retval;
      if((retval=poll(pfd,2,1000)))
      {
        perror("ERROR in polling");
        exit(0);
      }
      if(retval>0)
      {
        char buffer[128];

        for(int i=0;i<2;i++)
        {
          if(pfd[i].revents&POLLIN)
          {
            dup2(fd[i],0);
            scanf("%s\n",buffer);
            fflush(stdout);
            fflush(0);
            printf("%s\n",buffer);
          }
        }
      }
      else
        printf("still waiting\n");
    }
}
