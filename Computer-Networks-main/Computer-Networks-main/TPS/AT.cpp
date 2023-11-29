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

struct candidate{
    char name[100];
    int roll;
    int done[3];
};

bool* avail;
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

void* clientHandler(void* args){
    int nsfd = *(int*)args;
    char buff[sizeof(candidate)];
    struct candidate c;
    read(nsfd,buff,sizeof(buff));
    memcpy(&c,buff,sizeof(candidate));
    int score = rand()%11+1;
    cout << c.name << " " << score << endl;
    if(score > 0){
        int passed = 1;
        send(nsfd,&passed,sizeof(passed),0);
        bool fdPassed = 0;
        while(!fdPassed){
            for(int i = 0; i < 3 && !fdPassed; ++i){
                if(avail[i] && !c.done[i]){
                    fdPassed = true;
                    avail[i] = false;
                    c.done[i] = true;
                    int usfd = socket(AF_UNIX,SOCK_STREAM,0);
                    struct sockaddr_un addr;
                    addr.sun_family = AF_UNIX;
                    strcpy(addr.sun_path,to_string(8081+i).c_str());
                    if(connect(usfd,(struct sockaddr *)&addr,sizeof(addr))==-1)perror("connect ");
                    else{
                        //sendfd()
                        int res = send_fd(usfd,nsfd);
                        cout << "sendfd res : " << res << endl;
                        //sendCandidateStruct  
                        memcpy(buff,&c,sizeof(candidate));
                        send(usfd,buff,sizeof(buff),0);
                        //ye dekhle
                        // read(usfd,&res,sizeof(int));
                        // close(nsfd);
                    }
                }
            }
        }
    }else{
        int passed = 0;
        send(nsfd,&passed,sizeof(passed),0);
    }
}

int main(){
    srand(time(0));
    int shmid = shmget(123,3*sizeof(bool),0666|IPC_CREAT);
    avail = (bool*)shmat(shmid,NULL,0);
    for(int i = 0; i < 3; ++i)avail[i] = true;

    int sfd = sfdMaker(8000);

    pthread_t pt[5];
    for(int i = 0; i < 5; ++i){
        sockaddr_in clien;
        int clienlen = sizeof(clien);
        int nsfd = accept(sfd,(sockaddr*)& clien, (socklen_t*) &clienlen);
        int* fixed = (int*)malloc(sizeof(int));
        *fixed = nsfd;
        pthread_create(&pt[i],NULL,clientHandler,fixed);
    }
    for(int i = 0; i < 5; ++i)pthread_join(pt[i],NULL);
    cout << " ? bruh ? \n";
    return 0;
}