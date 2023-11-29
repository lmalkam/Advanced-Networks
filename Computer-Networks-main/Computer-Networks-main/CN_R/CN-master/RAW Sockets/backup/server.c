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
int *shm;
int nusfd[10],cnt=0;
key_t key=10;
int busfd,stop = 1;
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
  shm[1] = getpid();
  shm[5] = cnt;
  kill((pid_t)shm[15],SIGUSR1);
  printf("count:%d\n",cnt);
	for(int i=0;i<cnt;i++)
	{
		shm[1]=getpid();
		if(i==cnt-1)
		{
		  shm[2]=getpid();
		}
		send_fd(busfd,nusfd[i]);
		
    printf("fired\n");
	}
	while(stop);
}
void fn2()
{

	shm[1]=getpid();
	kill(shm[15],SIGUSR2);
	sleep(1);
	for(int i=0;i<shm[3];i++)
	{
		nusfd[cnt++]=recv_fd(busfd);
	}
	stop=0;
}
int main(int argc, char const *argv[])
{
	if(argc!=2)
	{
		printf("Enter ufd file name\n");
		return 0;
	}
	int shmid=shmget(key,20,IPC_CREAT|0666);
    shm=shmat(shmid,(void*)0,0);
    shm[2]=0;
    shm[1]=getpid();
    printf("pid:%d\n",shm[1]);
    //----backup connection creation.............................
    {
	    #define ADDRESS     "backup"
		struct sockaddr_un buserv_addr;
		int buserv_len,bucli_len;

		busfd = socket(AF_UNIX, SOCK_STREAM, 0);

		if(busfd<0){perror("\nsocket  ");}

		bzero(&buserv_addr,sizeof(buserv_addr));
		buserv_addr.sun_family = AF_UNIX;
		strcpy(buserv_addr.sun_path, ADDRESS);
		buserv_len = sizeof(buserv_addr);
		if(connect(busfd,(struct sockaddr *)&buserv_addr,buserv_len)==-1)
	     perror("\n connect ");
	 	else
	 	 printf("unix sockets connect successfull\n");
 	}
 	//--------------------------------------------------------------
 	signal(SIGUSR1,fn1);
    signal(SIGUSR2,fn2);

    struct sockaddr_un userv_addr,ucli_addr;
    int userv_len,ucli_len;
    int usfd;

    if((usfd = socket(AF_UNIX , SOCK_STREAM , 0))<0){perror("socket");};
    bzero(&userv_addr,sizeof(userv_addr));

   	userv_addr.sun_family = AF_UNIX;
 	strcpy(userv_addr.sun_path, argv[1]);
 	userv_len = sizeof(userv_addr);
 	unlink(argv[1]);
 	if(bind(usfd, (struct sockaddr *)&userv_addr, userv_len)==-1)
 		perror("server: bind");
 	listen(usfd, 5);
 	ucli_len=sizeof(ucli_addr);

 	fd_set rfd;

 	while(1)
 	{
 		FD_ZERO(&rfd);
 		FD_SET(usfd,&rfd);
 		for(int i=0;i<cnt;i++)
   		{
	     	FD_SET(nusfd[i],&rfd);
	    }

	    int s=select(FD_SETSIZE,&rfd,NULL,NULL,NULL);
	    if(s>0)
	    {
	    	if(FD_ISSET(usfd,&rfd))
		    {
				nusfd[cnt++]=accept(usfd,NULL,NULL);
				printf("\n Accepted \n");
		    }
		    for(int i=0;i<cnt;i++)
       		{
				if(FD_ISSET(nusfd[i],&rfd))
				{
					char buff[100];
					read(nusfd[i],buff,100);
					printf("\n Msg recieved %s \n",buff);
					for(int j=0;j<cnt;j++)
					{
						if(i!=j)
						send(nusfd[j],buff,strlen(buff),0);
					}
				}
       		}
	    }
 	}
 
 	return 0;
}