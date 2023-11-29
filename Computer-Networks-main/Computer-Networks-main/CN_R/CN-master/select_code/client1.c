#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#define server_pipe "server_pipe"
#define client "./client1"

int read_fd,write_fd;

void init_fifo()
{
  mkfifo(server_pipe,0666);
  mkfifo(client,0666);
}

int sigint(int sig)
{
  close(read_fd);
  close(write_fd);
  exit(0);
}

int main()
{
  init_fifo();
  int pid=fork();
  char buffer[20];

  while(1)
  {
    memset(buffer, 0, sizeof(buffer));
    if(pid>0)
    {
      write_fd=open(server_pipe,O_RDWR | O_NONBLOCK);
      read(0,buffer,20);
      //buffer[strlen(buffer)]='\0';
      printf("The data entered is %s\n",buffer);
      write(write_fd,buffer,strlen(buffer));
      close(write_fd);
    }
    else if(pid==0)
    {
      int n=0;
      read_fd=open(client,O_RDWR | O_NONBLOCK);
      while(n=read(read_fd,buffer,20)>0)
      {
        //buffer[strlen(buffer)]='\0';
        printf("Data received from client %s\n",buffer);
      }
      close(read_fd);
    }
  }
}
