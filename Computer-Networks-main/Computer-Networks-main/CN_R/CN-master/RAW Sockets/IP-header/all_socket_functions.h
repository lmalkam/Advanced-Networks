#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<netdb.h>
#include<sys/select.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<semaphore.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>

#define QUIT "quit"
#define CONTROLLEN  CMSG_LEN(sizeof(int))
#define MAXLINE 1024



/*this function create a udp server which will provide connection to the udp type clients and return sfd on success */
int create_udp_server(int port)
{
  struct sockaddr_in server_addr,client_addr;
  socklen_t server_len,client_len;
  int sfd;

  if((sfd=socket(AF_INET,SOCK_DGRAM,0))<0)
  {
    perror("Error while creating udp socket");
    exit(0);
  }

  server_addr.sin_family=AF_INET;
  server_addr.sin_addr.s_addr=INADDR_ANY;
  server_addr.sin_port=htons(port);
  bzero((void*)(&server_addr.sin_zero),0);

  server_len=sizeof(server_addr);

  if(bind(sfd,(struct sockaddr*)(&server_addr),server_len)<0)
  {
    perror("Error while bind udp socket");
    exit(0);
  }

  return sfd;

}

/*this function will create a udp client and will return the sfd created on success*/
int create_udp_client()
{
  int sfd;

  if((sfd=socket(AF_INET,SOCK_DGRAM,0))<0)
  {
    perror("Error while creating udp socket");
    exit(0);
  }

  return sfd;
}

/*this is used for the getting the server address in case of udp and tcp protocol*/
struct sockaddr_in get_server_addr(int port)
{
  struct sockaddr_in server_addr;

  server_addr.sin_family=AF_INET;
  server_addr.sin_port=htons(port);
  server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
  bzero((void *)(&(server_addr.sin_zero)),8);

  return server_addr;
}

/*This function is used for creating a tcp server and doing everything and printing an error message in case of error else return an sfd*/
int create_tcp_server(int port)
{
  struct sockaddr_in server_addr;
  socklen_t server_len;
  int opt=1,sfd;

  if((sfd=socket(AF_INET,SOCK_STREAM,0))==-1)
  {
    perror("Error while creating socket");
    exit(0);
  }

  if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

  server_addr.sin_family=AF_INET;
  server_addr.sin_port=htons(port);
  server_addr.sin_addr.s_addr=INADDR_ANY;
  bzero((void *)(&(server_addr.sin_zero)),8);

  server_len=sizeof(server_addr);
  if((bind(sfd,(struct sockaddr*)(&server_addr),server_len))==-1)
  {
    perror("Error while binding the address");
    exit(0);
  }

  if(listen(sfd,5)==-1)
  {
    perror("Error while listening");
    exit(0);
  }
  return sfd;
}

/*This function is used for creating a tcp client ehich connects to a given port no of server and returns back the sfd on success*/
int create_tcp_client(int port)
{
  struct sockaddr_in server_addr;
  socklen_t server_len;
  int sfd;

  if((sfd=socket(AF_INET,SOCK_STREAM,0))==-1)
  {
    perror("Error while creating socket");
    exit(0);
  }

  server_addr=get_server_addr(port);

  server_len=sizeof(server_addr);

  server_len=sizeof(server_addr);
  if(connect(sfd,(struct sockaddr*)(&server_addr),server_len)==-1)
  {
    perror("Error while connecting");
    exit(0);
  }
  return sfd;
}

/*This is used to accept the tcp clients coming for the connection*/
int accept_tcp_clients(int sfd)
{
  struct sockaddr_in client_addr;
  socklen_t client_len =  sizeof(client_addr);
  int nsfd;
  if ((nsfd = accept(sfd,(struct sockaddr *)&client_addr,&client_len))<0)
  {
		perror("Error While accepting the client");
		exit(0);
	}
	return nsfd;
}

//this is for the communication with the clinet in case of coonection established

void handle_tcp_client(int nsfd)
{
  while (1)
  {
    fflush(stdout);
    int bytes_recv,bytes_sent;
    char buffer_send[1024];
    char buffer_recv[1024];
    bytes_recv=recv(nsfd,&buffer_recv,1024,0);
    if(strcmp(buffer_recv,QUIT)==0)
    {
      close(nsfd);
      break;
    }
    buffer_recv[bytes_recv]='\0';
    printf("Received Message: %s\n",buffer_recv);
    printf("Press q to QUIT the connection\n");
    printf("Enter the data to be sent:");
    fgets(buffer_send,1024,stdin);
    if(strcmp(buffer_send,QUIT)==0)
    {
      close(nsfd);
      break;
    }
    if((bytes_sent=send(nsfd,&buffer_send,strlen(buffer_send),0))==-1)
    {
      perror("Send has failed");
      exit(0);
    }

  }
}


void handle_tcp_server(int nsfd)
{
  while (1)
  {
    int bytes_recv,bytes_sent;
    char buffer_send[1024];
    char buffer_recv[1024];

    printf("Press q to QUIT the connection\n");
    printf("Enter the data to be sent:");
    fgets(buffer_send,1024,stdin);
    if(strcmp(buffer_send,QUIT)==0)
    {
      close(nsfd);
      break;
    }
    if((bytes_sent=send(nsfd,&buffer_send,strlen(buffer_send),0))==-1)
    {
      perror("Send has failed");
      exit(0);
    }

    bytes_recv=recv(nsfd,&buffer_recv,1024,0);
    if(strcmp(buffer_recv,QUIT)==0)
    {
      close(nsfd);
      break;
    }
    buffer_recv[bytes_recv]='\0';
    printf("Received Message: %s\n",buffer_recv);

  }
}

/*This functions returns back the selected file descriptor on success*/
int select_fds(int fds[],int n)
{
  fd_set read_fds;
  int retval,max_fd;
  for(int i=0;i<n;i++)
    max_fd=max_fd>fds[i]?max_fd:fds[i];


  struct timeval tv;
  retval=-1;
  do
  {
    FD_ZERO(&read_fds);
    tv.tv_sec=5;
    tv.tv_usec=0;
    for(int i=0;i<n;i++)
      FD_SET(fds[i],&read_fds);
    retval=select(max_fd+1,&read_fds,NULL,NULL,&tv);
  }while(retval==-1);

  if(retval==-1)
  {
    perror("ERROR in select");
    return -1;
  }
  else
  {
    for(int i=0;i<n;i++)
    {
      if(FD_ISSET(fds[i],&read_fds))
        return i;
    }
    return -1;
  }
}



/*This function creates a Unix Domain Socket server and returns back a sfd on success*/
int create_uds_server(char* address)
{
  struct sockaddr_un userver_addr;
  socklen_t userver_len;
  int usfd;

  if((usfd=socket(AF_UNIX,SOCK_STREAM,0))<0)
  {
    perror("Error while creating a UDS socket");
    exit(0);
  }

  bzero(&userver_addr,sizeof(userver_addr));
  userver_addr.sun_family=AF_UNIX;
  strcpy(userver_addr.sun_path,address);
  unlink(address);
  userver_len=sizeof(userver_addr);

  if((bind(usfd,(struct sockaddr*)(&userver_addr),userver_len))==-1)
  {
    perror("Error while binding the address in USD");
    exit(0);
  }

  if(listen(usfd,5)==-1)
  {
    perror("Error while listening");
    exit(0);
  }
  return usfd;

}

/*This function creates a Unix Domain Client and returns back the usfd on success*/
int create_uds_client(char* address)
{
  struct sockaddr_un userver_addr;
  socklen_t userver_len;
  int usfd;

  if((usfd=socket(AF_UNIX,SOCK_STREAM,0))<0)
  {
    perror("Error while creating a UDS socket");
    exit(0);
  }

  bzero(&userver_addr,sizeof(userver_addr));
  userver_addr.sun_family=AF_UNIX;
  strcpy(userver_addr.sun_path,address);
  userver_len=sizeof(userver_addr);

  if(connect(usfd,(struct sockaddr*)(&userver_addr),userver_len)==-1)
  {
    perror("Error while connecting");
    exit(0);
  }
  return usfd;
}

/*This is used to accept the uds clients coming for the connection*/
int accept_uds_client(int usfd)
{
  struct sockaddr_un uclient_addr;
  socklen_t uclient_len =  sizeof(uclient_addr);
  int nusfd;
  if ((nusfd = accept(usfd,(struct sockaddr *)&uclient_addr,&uclient_len))<0)
  {
		perror("Error While accepting the client");
		exit(0);
	}
	return nusfd;
}

/*this function is used to send the sfd over the usd sockets*/
int send_fd(int fd, int fd_to_send)
{
    struct iovec    iov[1];
    struct msghdr   msg;
    char            buf[2]; /* send_fd()/recv_fd() 2-byte protocol */
    static struct cmsghdr   *cmptr = NULL;  /* malloc'ed first time */

    iov[0].iov_base = buf;
    iov[0].iov_len  = 2;
    msg.msg_iov     = iov;
    msg.msg_iovlen  = 1;
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;
    if (fd_to_send < 0) {
        msg.msg_control    = NULL;
        msg.msg_controllen = 0;
        buf[1] = -fd_to_send;   /* nonzero status means error */
        if (buf[1] == 0)
            buf[1] = 1; /* -256, etc. would screw up protocol */
    } else {
        if (cmptr == NULL && (cmptr = (struct cmsghdr*)malloc(CONTROLLEN)) == NULL)
            return(-1);
        cmptr->cmsg_level  = SOL_SOCKET;
        cmptr->cmsg_type   = SCM_RIGHTS;
        cmptr->cmsg_len    = CONTROLLEN;
        msg.msg_control    = cmptr;
        msg.msg_controllen = CONTROLLEN;
        *(int *)CMSG_DATA(cmptr) = fd_to_send;     /* the fd to pass */
        buf[1] = 0;          /* zero status means OK */
    }
    buf[0] = 0;              /* null byte flag to recv_fd() */
    if (sendmsg(fd, &msg, 0) != 2)
        return(-1);
    return(0);
}

/*this is used for receiving the sfd over the uds sockets*/
int recv_fd(int fd)
{
   int             newfd, nr, status;
   char            *ptr;
   char            buf[MAXLINE];
   struct iovec    iov[1];
   struct msghdr   msg;
   static struct cmsghdr   *cmptr = NULL;  /* malloc'ed first time */
   status = -1;
   for ( ; ; ) {
       iov[0].iov_base = buf;
       iov[0].iov_len  = sizeof(buf);
       msg.msg_iov     = iov;
       msg.msg_iovlen  = 1;
       msg.msg_name    = NULL;
       msg.msg_namelen = 0;
       if (cmptr == NULL && (cmptr = (struct cmsghdr*)malloc(CONTROLLEN)) == NULL)
           return(-1);
       msg.msg_control    = cmptr;
       msg.msg_controllen = CONTROLLEN;
       if ((nr = recvmsg(fd, &msg, 0)) < 0) {
           perror("recvmsg error");
       }
       else if (nr == 0) {
           perror("connection closed by server");
           return(-1);
       }
       for (ptr = buf; ptr < &buf[nr]; ) {
           if (*ptr++ == 0) {
               if (ptr != &buf[nr-1])
                   perror("message format error");
               status = *ptr & 0xFF;  /* prevent sign extension */
               if (status == 0) {
                   if (msg.msg_controllen != CONTROLLEN)
                       perror("status = 0 but no fd");
                   newfd = *(int *)CMSG_DATA(cmptr);
               } else {
                   newfd = -status;
               }
               nr -= 2;
           }
        }

        if (status >= 0)    /* final data has arrived */
            return(newfd);  /* descriptor, or -status */
        return -1;
   }
}

int create_raw_socket(char* address)
{
  int opt=1,rsfd;

  if((rsfd=socket(AF_INET,SOCK_RAW,101))<0)
  {
    perror("Error While creating the raw socket");
    exit(0);
  }

  if(setsockopt(rsfd,IPPROTO_IP,SO_BROADCAST,&opt,sizeof(int))<0)
  {
    perror("Error in set sock opt");
    exit(0);
  }

  struct sockaddr_in server_addr;
  socklen_t server_len;

  server_addr.sin_family=AF_INET;
  server_addr.sin_addr.s_addr=inet_addr(address);
  server_len=sizeof(server_addr);

  if(bind(rsfd,(struct sockaddr*)&server_addr,server_len)<0)
  {
    perror("Error while binding");
    exit(0);
  }
  return rsfd;

}
