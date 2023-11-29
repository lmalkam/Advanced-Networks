#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/poll.h>
#include <string.h>
#include<errno.h>

#define client1 "./client_1"
#define serverpipe "./server_pipe"

int server_fd,read_fd;

void request_server()
{
  printf("im here4\n");
  printf("%s\n",strerror(errno));
  server_fd=open(serverpipe,O_WRONLY);
  printf("%s\n",strerror(errno));
  printf("im here5\n");
  write(server_fd,client1,strlen(client1)+1);
  close(server_fd);
}

void write_to_server(int sig)
{
  printf("write a message::");
  server_fd=open(serverpipe,O_WRONLY);
  char arr[80];fgets(arr,80,stdin);
  //char str[80];strcpy(str,'1'+'-'+arr);
  printf("%s\n",arr);
  write(server_fd,arr,strlen(arr)+1);
  close(server_fd);
}

int main()
{
  printf("im here1\n");
  mkfifo(serverpipe,O_CREAT | O_EXCL | 0777);
  mkfifo(client1,O_CREAT | O_EXCL | 0777);
  printf("im here2\n");

  request_server();
  printf("im here3\n");
  char buf[80];
  while(1)
  {
    signal(SIGINT,write_to_server);

    read_fd=open(client1,O_RDONLY | O_NONBLOCK);
    if(read(read_fd,buf,80)>0)
      printf("received: %s",buf);
    close(read_fd);
  }
}
