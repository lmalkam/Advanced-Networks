#include<bits/stdc++.h>
#include<sys/poll.h>
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
#include<sys/ipc.h>
#include<sys/msg.h>
#include<iostream>
#include<semaphore.h>
#include<cctype>
#include<sys/stat.h>
using namespace std;
#define socketPath "mySocket"
int neighbour = -1;

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

int usfdMaker(){
    int usfd = socket(AF_UNIX,SOCK_STREAM,0);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path,socketPath);

    unlink(socketPath);
    if(bind(usfd,(sockaddr*)&addr,sizeof(addr)) < 0){
        perror("bind fail ");
        exit(0);
    }
    if(listen(usfd,5) < 0){
        perror("listen fail ");
        exit(0);
    }
    return usfd;
}

void hndfn(int sig, siginfo_t *siginfo, void* context){
    cout << "Sig1 recieved" << " " << neighbour << endl;
    int recvfd;
	if(neighbour == -1){
        struct sockaddr_in address;
        if((recvfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		    perror("sfd fail");
		    exit(0);
	    }
	
	    address.sin_family = AF_INET;
	    address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(8888);
	
	    if(connect(recvfd,(struct sockaddr*)&address,sizeof(address)) < 0){
		    perror("connect failed");
		    exit(0);
	    }
    }else{
        int usfd = socket(AF_UNIX,SOCK_STREAM,0);
        struct sockaddr_un addr;
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path,socketPath);
        if(connect(usfd,(struct sockaddr *)&addr,sizeof(addr))==-1)perror("\n connect ");
        recvfd = recv_fd(usfd);
        printf("fd received %d", recvfd);
        // close(usfd);
    }
    
    for(int i = 0; i < 5; ++i){
        char message[1000];
        read(recvfd,message,sizeof(message));
        cout << message << endl;
    }
    while(neighbour == -1){
        for(int i = 0; i < 5; ++i){
            char message[1000];
            read(recvfd,message,sizeof(message));
            cout << message << endl;
        }
    }
    cout << "DONE\n";
    int usfd = usfdMaker();
    sockaddr_un clien;
    socklen_t cliensize = sizeof(clien);
    kill(neighbour,SIGUSR1);
    int nsfd = accept(usfd,(sockaddr*)&clien,&cliensize);
    int res = send_fd(nsfd,recvfd);
    cout << "send fd res " << res << endl;
    // close(usfd);
}

void hndfn1(int sig, siginfo_t *siginfo, void* context){
	neighbour = siginfo->si_pid;
}

int main(int argc, char* argv[]){
    struct sigaction act;
    act.sa_sigaction = hndfn;
    act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	struct sigaction act1;
    act1.sa_sigaction = hndfn1;
    act1.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR2, &act1, NULL);

    int shmid1 = shmget(1234,sizeof(int),0666|IPC_CREAT);
    int shmid2 = shmget(12345,sizeof(int),0666|IPC_CREAT);
    int* headpid = (int*)shmat(shmid1,NULL,0);
    int* trailpid = (int*)shmat(shmid2,NULL,0);
    cout << *headpid << " " << *trailpid << endl;
    if(*headpid <= 0){
        *headpid = getpid();
        *trailpid = getpid();
        raise(SIGUSR1);
    }else{
        neighbour = *headpid;
        kill(*trailpid,SIGUSR2);
        *trailpid = getpid();
    }

    struct pollfd pfds[argc-1];
    for(int i = 1; i < argc; ++i){
        struct sockaddr_in address;
	
	    int sfd;
	    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		    perror("sfd fail");
		    return 0;
	    }
	
	    address.sin_family = AF_INET;
	    address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(8000+atoi(argv[i]));
	
	    if(connect(sfd,(struct sockaddr*)&address,sizeof(address)) < 0){
		    perror("connect failed");
		    return 0;
	    }
        pfds[i-1].fd = sfd;
        pfds[i-1].events = POLLIN;
        pfds[i-1].revents = 0;
    }

    while(1){
        int ne = poll(pfds,argc-1,1000*5);
        if(!ne)continue;
        for(int i = 0; i < argc-1; ++i){
            if(pfds[i].revents & POLLIN){
                char message[1000];
                read(pfds[i].fd,message,sizeof(message));
                write(STDOUT_FILENO,message,strlen(message)+1);
            }
        }
    }
    return 0;
}