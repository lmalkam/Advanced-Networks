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

bool* avail;

struct candidate{
    char name[100];
    int roll;
    int done[3];
};

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

int usfdMaker(char* portS){
    sockaddr_un serv;
    int usfd = socket(AF_UNIX,SOCK_STREAM,0);
    serv.sun_family = AF_UNIX;
    strcpy(serv.sun_path,portS);
    unlink(portS);
    if(bind(usfd,(sockaddr*)&serv,sizeof(serv)) < 0){
        perror("bind fail ");
        exit(0);
    }
    if(listen(usfd,5) < 0){
        perror("listen fail ");
        exit(0);
    }
    return usfd;
}

void* candidateWaiting(void* args){
    pair<int,candidate> arg = *(pair<int,candidate>*)args;
    candidate c = arg.second;
    int fd = arg.first;
    int allDone = 1;
    for(int i = 0; i < 3; ++i)allDone = allDone & c.done[i];
    if(!allDone){
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
                        send_fd(usfd,fd);
                        //sendCandidateStruct  
                        char buff[sizeof(candidate)];
                        memcpy(buff,&c,sizeof(candidate));
                        send(usfd,buff,sizeof(buff),0);
                    }
                }
            }
        }
    }
    return nullptr;
}


int main(int argc, char* argv[]){
    srand(time(0));
    int shmid = shmget(123,3*sizeof(bool),0666|IPC_CREAT);
    avail = (bool*)shmat(shmid,NULL,0);
    int usfd = usfdMaker(argv[1]);

    int nusfd;
    sockaddr_un clien;
    socklen_t cliensize = sizeof(clien);
    while((nusfd = accept(usfd,(sockaddr*)&clien,&cliensize)) > 0){
        char buff[sizeof(candidate)];
        candidate c;
        //recv_fd()
        int recvfd;
        if((recvfd = recv_fd(nusfd)) < 0){perror("ah ");break;}
        // int done = 1;
        // send(nusfd,&done,sizeof(int),0);
        read(nusfd,buff,sizeof(buff));
        memcpy(&c,buff,sizeof(candidate));
        cout << c.name << " " << recvfd << endl;
        char greet[] = "Hello How Are You ? ";
        char resp[1024];
        //send()
        send(recvfd,greet,strlen(greet)+1,0);
        //read()
        read(recvfd,resp,sizeof(resp));
        cout << resp << endl;
        int score = rand()%11;
        memcpy(buff,&c,sizeof(candidate));
        int usfdj = socket(AF_UNIX,SOCK_STREAM,0);
        struct sockaddr_un addr;
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path,"JP");
        if(connect(usfdj,(struct sockaddr *)&addr,sizeof(addr))==-1)perror("connect ");
        //send struct to J
        send(usfdj,buff,sizeof(buff),0);
        //send score to J   
        send(usfdj,&score,sizeof(int),0);

        pthread_t pt;
        pair<int,candidate>* pp = (pair<int,candidate>*)malloc(sizeof(pair<int,candidate>));
        pp->first = recvfd;
        pp->second = c; 
        pthread_create(&pt,NULL,candidateWaiting,pp);

        avail[atoi(argv[1])-8081] = true;
        cout << "FREE now\n";
    }

    return 0;
}