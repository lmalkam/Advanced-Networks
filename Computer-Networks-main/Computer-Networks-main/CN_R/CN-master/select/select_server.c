#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#define server_pipe "server_pipe"
#define total 3


int server_fd;
char fifos[total][15];
int fds[total];
int max_fd=-2;

void sigint(int sig)
{
  for(int i=0;i<total;i++)
    close(fds[i]);

  close(server_fd);
}

void init_fifo()
{
  char str[10];
  strcpy(str,"./client");
  for(int i=0;i<total;i++)
  {
    char val[5];
    sprintf(val,"%d",i+1);
    strcpy(fifos[i],str);
    strcat(fifos[i],val);
    printf("%s\n",fifos[i] );
    mkfifo(fifos,0666);
  }

void init_fds()
{
  for(int i=0;i<total;i++)
  {
    fds[i]=open(fifos[i],O_RDWR | O_NONBLOCK);
    if(fds[i]==-1)
      perror("Error Detected in FD's");
    max_fd=max_fd>fds[i]?max_fd:fds[i];
  }
}

int main()
{
  init_fifo();
  mkfifo(server_pipe,0666);
  server_fd=open(server_pipe,O_RDWR | O_NONBLOCK);

  fd_set rfds;
  struct timeval tv;
  int retval,wr;
  char buffer[20]={0};

  tv.tv_sec=5;tv.tv_usec=0;

  init_fds();
  int retval;

  while(1)
  {
    sleep(1);
    do
    {
      FD_ZERO(&rfds);
      FD_SET(server_fd,&rfds);
      retval=select(max_fd+1,&rfds,NULL,NULL,&tv);
    }while(retval==-1);

    if(retval>0)
    {
      for(int i=0;i<total;i++)
      {
        if(FD_ISSET(fds[i],&rfds))
        {
          read(fds[i],buffer,20);
          buffer[strlen(buffer)]="\0";
          printf("Client %d messaged %s\n",i+1,buffer );
          for(int j=0;j<total;j++)
          {
            if(j==i)
              continue;
            int temp_fd=open(fifos[i],O_RDWR | O_NONBLOCK);

            if(!temp_fd)
              perror("Error opening fds for write");

            write(temp_fd,buffer,strlen(buffer));
            close(temp_fd);
          }
        }
      }
    }
    else if(retval==0)
    printf("There is nothing to read\n");
    else
    {
      perror("Error faced by the select");
      break;
    }
  }
  return 0;
}
