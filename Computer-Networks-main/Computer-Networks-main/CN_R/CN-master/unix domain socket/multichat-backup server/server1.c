#include "all_socket_functions.h"
#define PORT 5050
#define SHARED "shmpid"
#define ADDRESS "udsocket"

int shmqid;
char* shared_buffer;
int sfd,usfd;
int backup_pid;
int sfd_count=0;
int fds[100];

//this function is used to initilize the shared memory that stores the pid of the backup server
void init_shm()
{
  key_t key=ftok(SHARED,101);
  shmqid=shmget(key,1024,0666|IPC_CREAT);
  shared_buffer=(char*)(shmat(shmqid,NULL,0));
  backup_pid=atoi(shared_buffer);
  kill(backup_pid,SIGUSR1);
}

void sigint(int sig)
{
  kill(backup_pid,SIGUSR1);
  for(int i=2;i<sfd_count;i++)
    send_fd(usfd,fds[i]);
}

int main()
{
  signal(SIGINT,sigint);
  //create a shared memory and get the backup pid from the shared memory
  init_shm();
  //create a tcp server
  sfd=create_tcp_server(PORT);
  //create a unix doamin server
  usfd=create_uds_client(ADDRESS);

  fds[0]=sfd;
  fds[1]=usfd;
  sfd_count=2;

  while(1)
  {
    int chosen_sfd=select_fds(fds,sfd_count);
    //a new client is trying to connect to the server
    if(chosen_sfd==0)
    {
      int nsfd=accept_tcp_clients(sfd);
      fds[sfd_count++]=nsfd;
      pid_t pid=fork();
      if(pid==0)
        handle_tcp_client(chosen_sfd);
      else
        close(nsfd);
    }
    else if(chosen_sfd==1)
    {
      fds[sfd_count++]=recv_fd(usfd);
    }
    else
    {
      pid_t pid=fork();
      if(pid==0)
        handle_tcp_client(chosen_sfd);
    }

  }
}
