/*
make 5 clients such that every client can have direct communication with the server to access the data from the exe file
but we need to take care that the clients first places their request to the server in a defined format and after that its provided
with the data from the database

make 6 different named pipes for the communication
3 different database and if database requested in not there create one
after the do the communication between the database and the client
*/


#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include <ctype.h>
#include<sys/poll.h>
#include<string.h>
#include<errno.h>
#define serverpipe "./server_pipe"

char  myfifo[10][20];
int client_count=0;
int server_fd,write_fd;

int check_presence(char* buf)
{
  if(buf[0]=='.')
  {
    strcpy(myfifo[client_count],buf);
    mkfifo(myfifo[client_count++],O_CREAT | O_EXCL | 0777);
    return 0;
  }
  else
    return 1;

}

void delete_server(int sig)
{
    close(server_fd);
}

int main()
{
   printf("im here1");
   mkfifo(serverpipe,O_CREAT | O_EXCL | 0777);
   printf("%s\n",strerror(errno));
   server_fd=open(serverpipe,O_RDONLY | O_NONBLOCK);
   printf("%s\n",strerror(errno));
   printf("im here2");

  while(1)
  {
    //printf("im here\n");
    signal(SIGINT,delete_server);
    char buf[80];

    if(read(server_fd,buf,80)>0)
    {
      if(check_presence(buf))
      {
        printf("Message transfer:: %s\n",buf);
        for(int i=0;i<client_count;i++)
        {
          printf("rahul here\n" );
           // if(buf[0]==myfifo[i][8])
           //   continue;

          printf("%s\n",strerror(errno));
          write_fd=open(myfifo[i],O_WRONLY);
          write(write_fd,buf,strlen(buf)+1);
          printf("%s\n",strerror(errno));
          close(write_fd);
        }
      }
      else
        printf("New client joined the chat server:: %s\n",myfifo[client_count-1]);
    }
  }

}
