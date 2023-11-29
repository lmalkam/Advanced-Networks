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

#define socketPath "GG"

int sfd;

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

int getWinner(struct pollfd pfds[], int sz){
    int ne = poll(pfds,sz,1000*3);
    if(!ne)return 0;
    for(int i = 0; i < sz; ++i){
        if(pfds[i].revents & POLLIN){
            int win = 1;
            send(pfds[i].fd,&win,sizeof(win),0);
            break;
        }
    }
    return 1;
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

int main(){
    srand(time(0));
    int usfd = usfdMaker();
    sockaddr_un clien;
    socklen_t cliensiz = sizeof(clien);
    int nusfd = accept(usfd,(sockaddr*)&clien,&cliensiz);
    int sz;
    read(nusfd,&sz,sizeof(sz));
    cout << sz << endl;

    struct pollfd pfds[sz];
    vector<int> nsfds;
    for(int i = 0; i < sz; ++i){
        int nsfd = recv_fd(nusfd);
        nsfds.push_back(nsfd);
        pfds[i].fd = nsfd;
        pfds[i].events = POLLIN;
        pfds[i].revents = 0;
    }



    bool done[120];
    memset(done,false,sizeof(done));

    for(int i = 0; i < 100; ++i){
        int num;
        while(done[num = (rand()%99+1)]){}
        done[num] = true;
        cout << num << endl;
        for(auto nsfd : nsfds)
            send(nsfd,&num,sizeof(int),0);
        int won = getWinner(pfds,sz);
        if(won){
            cout << "We have a Winner\n";
            num = -1;
            for(auto nsfd : nsfds)
                send(nsfd,&num,sizeof(int),0);
            break;
        }
    }
    return 0;
}