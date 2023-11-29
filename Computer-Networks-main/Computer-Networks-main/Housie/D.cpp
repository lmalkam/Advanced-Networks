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

int totalClients = 0;
int sfd;
#define socketPath "GG"

int sfdMaker(int port){
    struct sockaddr_in AT;
    int sfd;
	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("sfd fail");
		return 0;
	}
	int opt = 1;
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        	perror("setsockopt");
        	return 0;
    }
	AT.sin_family = AF_INET;
	AT.sin_addr.s_addr = INADDR_ANY;
	AT.sin_port = htons(port);
	if(bind(sfd,(struct sockaddr*) &AT, sizeof(AT)) < 0){
		perror("bind failed");
		return 0;
	}
	if(listen(sfd,3) < 0){
		perror("listen failed");
		return 0;
	}
    return sfd;
}

int* makeCopon(){
    int* copon = (int*)malloc(4*sizeof(int));
    for(int i = 0; i < 4; ++i){
        copon[i] = (rand()%99)+1;
    }
    return copon;
}

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


int main(){
    srand(time(0));
    sfd = sfdMaker(8000);

    vector<int> nsfds;
    
    struct pollfd pfd[1];
    pfd[0].fd = sfd;
    pfd[0].events = POLLIN;
    pfd[0].revents = 0;

    while(1){
        int ne = poll(pfd,1,1000*10);
        if(!ne)break;
        int nsfd;
        sockaddr_in clien;
        socklen_t cliensize = sizeof(clien);
        nsfd = accept(sfd,(sockaddr*)&clien,&cliensize);
        ++totalClients;
        nsfds.push_back(nsfd);
        int cn;
        read(nsfd,&cn,sizeof(cn));
        for(int i = 0; i < cn; ++i){
            int* copon = makeCopon(); 
            write(nsfd,copon,4*sizeof(int));
        }
    }

    int usfd = socket(AF_UNIX,SOCK_STREAM,0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path,socketPath);
    if(connect(usfd,(struct sockaddr *)&addr,sizeof(addr))==-1)perror("\n connect ");

    int sz = nsfds.size();
    cout << sz << endl;
    send(usfd,&sz,sizeof(int),0);

    for(auto nsfd : nsfds)
        send_fd(usfd,nsfd);
        // send(usfd,&nsfd,sizeof(int),0);

    cout << "? d ?\n";
    return 0;
}