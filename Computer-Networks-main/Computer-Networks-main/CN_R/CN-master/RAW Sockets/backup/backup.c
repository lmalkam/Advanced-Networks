#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>

int *p1;
key_t key=10;

struct server{
	int pid;
	int cli_sfd[10];
	int cli_no;
	int nusfd;
	int ncli_no;
	int usfd;
}chat[10];
int  nusfd[10];  //accepted sfd of backup server and chat server
int cnt=0; //No of chat servers sent their clients
int cspid[10];  // stoing chat server pids
int clsfd[10][10];  // storing chat servers clients
int clcnt[10];  //client count of each server
int nclcnt[10]; //new coming clients count
int cusfd[10]; // usfd of char servers
int stop[10];    // which chat service to stop working

int send_fd(int socket1, int fd_to_send){
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  /* storage space needed for an ancillary element with a paylod of length is CMSG_SPACE(sizeof(length)) */
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
  int available_ancillary_element_buffer_space;

  /* at least one vector of one byte must be sent */
  message_buffer[0] = 'F';
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;

  /* initialize socket message */
  memset(&socket_message, 0, sizeof(struct msghdr));
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
  memset(ancillary_element_buffer, 0, available_ancillary_element_buffer_space);
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = available_ancillary_element_buffer_space;

  /* initialize a single ancillary data element for fd passing */
  control_message = CMSG_FIRSTHDR(&socket_message);
  control_message->cmsg_level = SOL_SOCKET;
  control_message->cmsg_type = SCM_RIGHTS;
  control_message->cmsg_len = CMSG_LEN(sizeof(int));
  *((int *) CMSG_DATA(control_message)) = fd_to_send;

  return sendmsg(socket1, &socket_message, 0);}

int recv_fd(int socket1)
 {
  int sent_fd, available_ancillary_element_buffer_space;
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];

  /* start clean */
  memset(&socket_message, 0, sizeof(struct msghdr));
  memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));

  /* setup a place to fill in message contents */
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = CMSG_SPACE(sizeof(int));

  if(recvmsg(socket1, &socket_message, MSG_CMSG_CLOEXEC) < 0)
   return -1;

  if(message_buffer[0] != 'F')
  {
   /* this did not originate from the above function */
   return -1;
  }

  if((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
  {
   /* we did not provide enough space for the ancillary element array */
   return -1;
  }

  /* iterate ancillary elements */
   for(control_message = CMSG_FIRSTHDR(&socket_message);
       control_message != NULL;
       control_message = CMSG_NXTHDR(&socket_message, control_message))
  {
   if( (control_message->cmsg_level == SOL_SOCKET) &&
       (control_message->cmsg_type == SCM_RIGHTS) )
   {
    sent_fd = *((int *) CMSG_DATA(control_message));
    return sent_fd;
   }
  }

  return -1;
 }

void fn1()
{
	sleep(1);
	int flag=1;
	int chek = p1[5];
	for(int j=0;j<chek;j++)
	{

  	for(int i=0;i<cnt;i++)
  	{
  		if(chat[i].pid == p1[1])
  		{
  			flag = 0;
  			chat[i].cli_sfd[chat[i].cli_no++] = recv_fd(chat[i].nusfd);
  			printf("\n Client recived from %d and count is %d \n",p1[1],chat[i].cli_no);
    		write(chat[i].cli_sfd[chat[i].cli_no-1],"came",5);
  		}
  	}
  	if(p1[2]==p1[1]){p1[2]=0;}
  	if(flag==1)
  {
    printf("\n Not Found \n");
  }}

}

void fn2()
{
	for(int i=0;i<cnt;i++)
	{
		if(chat[i].pid==p1[1])
		{
			stop[i]=1;
			p1[3]=chat[i].ncli_no;
			for(int j=chat[i].cli_no;j<chat[i].cli_no+chat[i].ncli_no;j++)
			{
				send_fd(chat[i].nusfd,chat[i].cli_sfd[j]);
			}
		}
	}
}
int main(int argc, char const *argv[])
{
	for(int i=0;i<10;i++)
	{
		chat[i].pid = 0;
		chat[i].cli_no = 0;
		chat[i].nusfd = 0;
		chat[i].ncli_no = 0;
		stop[i]=0;
		for(int j=0;j<10;j++)
		{
		  chat[i].cli_sfd[j]=0;
		}
	}
	signal(SIGUSR1,fn1);
  	signal(SIGUSR2,fn2);
  	int shmid=shmget(key,20,IPC_CREAT|0666);
  	p1=shmat(shmid,(void*)0,0);
  	p1[15]=getpid();

  	//--------unix socket creation-------------------------------

  	#define ADDRESS     "backup"
	struct sockaddr_un userv_addr,ucli_addr;
	int userv_len,ucli_len;

	int usfd = socket(AF_UNIX , SOCK_STREAM , 0);
	if(usfd<0){perror("socket");}

	bzero(&userv_addr,sizeof(userv_addr));

	userv_addr.sun_family = AF_UNIX;
	strcpy(userv_addr.sun_path, ADDRESS);
	userv_len = sizeof(userv_addr);
	unlink(ADDRESS);
	if(bind(usfd, (struct sockaddr *)&userv_addr, userv_len)==-1)
		perror("server: bind");

	listen(usfd, 5);
	ucli_len=sizeof(ucli_addr);

	//---------------------------------------------------------------
	fd_set rfd;
	while(1)
	{
		FD_ZERO(&rfd);
		FD_SET(usfd,&rfd);
		for(int i=0;i<cnt;i++)
		{
			for(int j=0;j<chat[i].cli_no+chat[i].ncli_no;j++)
			{
				FD_SET(chat[i].cli_sfd[j],&rfd);
			}
		}
		for(int i=0;i<cnt;i++)
		{
			FD_SET(chat[i].usfd,&rfd);
		}
		int s=select(FD_SETSIZE,&rfd,NULL,NULL,NULL);
		printf("s:%d\n",s);
		if(s>0)
		{
			if(FD_ISSET(usfd,&rfd))
			{
				chat[cnt].pid=p1[1];
				chat[cnt].nusfd=accept(usfd,NULL,NULL);
				struct sockaddr_un userv_addr,ucli_addr;
         		int userv_len,ucli_len;
         		if((chat[cnt].usfd = socket(AF_UNIX , SOCK_STREAM , 0))<0){perror("socket()");}
         		bzero(&userv_addr,sizeof(userv_addr));

          		userv_addr.sun_family = AF_UNIX;
          		char c[100]={0};	
          		sprintf(c,"bserver%d",cnt+1);
        		strcpy(userv_addr.sun_path,c);
        		userv_len = sizeof(userv_addr);
        		unlink(c);

        		if(bind(chat[cnt].usfd,(struct sockaddr *)&userv_addr,userv_len)<0){perror("bind()");}
        		listen(chat[cnt].usfd,5);
        		cnt++;
			}
			for(int i=0;i<cnt;i++)
			{
				if(stop[i]==0)
				{
					if(FD_ISSET(chat[i].usfd,&rfd))
					{
						int p=chat[i].cli_no+chat[i].ncli_no;
				        chat[i].ncli_no++;
				        chat[i].cli_sfd[p]=accept(chat[i].usfd,NULL,NULL);
				        printf("\n accepted \n");
					}
				}
			}
			for(int i=0;i<cnt;i++)
			{
				if(stop[i]==0)
				{
					for(int j=0;j<chat[i].cli_no+chat[i].ncli_no;j++)
					{
						if(FD_ISSET(chat[i].cli_sfd[j],&rfd))
						{
							char buffer[100];
							printf("\n message recived from group %d c%d  \n",i+1,j+1);
            				read( chat[i].cli_sfd[j] , buffer, 100);
            				for(int k=0;k<chat[i].cli_no+chat[i].ncli_no;k++)
            				{
                				if(k!=j)
                				send(chat[i].cli_sfd[k] , buffer , strlen(buffer) , 0 );
            				}
						}
					}
				}
			}
		}
	}	
	return 0;
}
