#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <netdb.h>
#include<sys/select.h>
#include <arpa/inet.h>
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

  if(bind(sfd,(struct sockaddr_in*)(&server_addr),server_len)<0)
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
  struct sockaddr_in server_addr,client_addr;

  host=gethostbyname("127.0.0.1");
  server_addr.sin_family=AF_INET;
  server_addr.sin_port=htons(PORT);
  server_addr.sin_addr=*(struct in_addr*)host->h_addr;
  bzero((void *)(&(server_addr.sin_zero)),8);

}

/*This function is used for creating a tcp server and doing everything and printing an error message in case of error else return an sfd*/
int create_tcp_server(int port)
{
  struct sockaddr_in server_addr;
  socklen_t server_len;
  int opt=1;

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
  server_addr.sin_port=htons(PORT);
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
  struct hostent *host;

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

/*This functions returns back the selected file descriptor on success*/
int select_fds(int fds[],int n)
{
  fd_set read_fds;
  int retval,max_fd;
  for(int i=0;;i<n;i++)
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
    retval=select(max_sfd+1,&read_fds,NULL,NULL,&tv);
  }while(retval==-1);

  if(retval==-1)
  {
    perror("ERROR in select");
    exit(EXIT_FAILURE);
  }
  else
  {
    for(int i=0;i<n;i++)
    {
      if(FD_ISSET(fds[i],&read_fds))
        return i;
    }
  }
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

/*This function creates a Unix Domain Socket server and returns back a sfd on success*/
int create_uds_server(char* address)
{
  struct sockaadr_un userver_addr;
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
  struct sockaadr_un userver_addr;
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
int accept_uds_clients(int sfd)
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
int send_fd(int socket, int fd_to_send)
 {
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

  return sendmsg(socket, &socket_message, 0);
 }

/*this is used for receiving the sfd over the uds sockets*/

 int recv_fd(int socket)
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

  if(recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
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
