#include "all_socket_functions.h"
#define SHARED "shmpid"
#define ADDRESS "udsocket"

int shmqid;
char* shared_buffer;
int usfd;
int sfd_count=0;
int fds[100];
int server_no[100];
int server_status[2];
int server_count=0;

//this function is used to initilize the shared memory that stores the pid of the backup server
void init_shm()
{
  key_t key=ftok(SHARED,101);
  shmqid=shmget(key,1024,0666|IPC_CREAT);
  shared_buffer=(char*)(shmat(shmqid,NULL,0));
  sprintf(shared_buffer,"%d",getpid());
}

void sigusr1(int sig)
{
  if(server_status[0]==0)
  {
    while (1)
    {
      int chosen_sfd=select_fds(fds,sfd_count);
      if(chosen_sfd==fds[0]||chosen_sfd==fds[1])
      {
        fds[sfd_count++]=recv_fd(chosen_sfd);
        server_no[sfd_count-1]=chosen_sfd==fds[0]?0:1;
      }
      else
      {
        pid_t pid=fork();
        if(pid==0)
        {
          handle_tcp_client(chosen_sfd);
        }
      }
    }
  }
  else
  {
    for(int i=2;i<sfd_count;i++)
    {
      if(server_no[i]==0)
        send_fd(fds[0],fds[i]);
    }
  }
}

void sigusr2(int sig)
{
  if(server_status[0]==0)
  {
    while (1)
    {
      int chosen_sfd=select_fds(fds,sfd_count);
      if(chosen_sfd==fds[0]||chosen_sfd==fds[1])
      {
        fds[sfd_count++]=recv_fd(chosen_sfd);
        server_no[sfd_count-1]=chosen_sfd==fds[0]?0:1;
      }
      else
      {
        pid_t pid=fork();
        if(pid==0)
        {
          handle_tcp_client(chosen_sfd);
        }
      }
    }
  }
  else
  {
    for(int i=2;i<sfd_count;i++)
    {
      if(server_no[i]==1)
        send_fd(fds[1],fds[i]);
    }
  }
}

int main()
{
  server_status[0]=0;
  server_status[1]=0;
  //initilize the shared memory
  init_shm();
  usfd=create_uds_server(ADDRESS);
  while(1)
  {
    int pid;
    int nusfd=accept_uds_client(usfd);
    if(nusfd)
    {
      server_no[sfd_count++]=nusfd;
    }
  }
}
