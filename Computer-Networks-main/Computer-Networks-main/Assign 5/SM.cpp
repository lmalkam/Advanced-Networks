#include<bits/stdc++.h>
#include <sys/un.h>
#include<sys/socket.h>
#include<semaphore.h>
#include<sys/poll.h>
#include <fcntl.h>

using namespace std;

char trainPath[] = "808#";
char platformPath[] = "8000";
stack<int> availablePlatforms;

int send_fd(int socket, int fd_to_send){
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

int usfdMaker(char* portPath){
    int usfd = socket(AF_UNIX,SOCK_STREAM,0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path,portPath);
    unlink(portPath);
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


void* platformListener(void* args){
    int usfd = *(int*)args;
    sockaddr_un clien;
    socklen_t clienlen = sizeof(clien);
    int nusfd = accept(usfd,(sockaddr*)&clien,&clienlen);
    cout << "PConnected\n";
    availablePlatforms.push(nusfd);
    int done;

    while(1){
        read(nusfd,&done,sizeof(int));
        availablePlatforms.push(nusfd);
    }

}


int main(int argc, char* argv[]){
    vector<int> trainListener;
    for(int i = 0; i < 3; ++i){
        trainPath[3] = '1'+i;
        int usfd = usfdMaker(trainPath);
        trainListener.push_back(usfd);
    }
   
    vector<pthread_t> pts(3);
    int usfd = usfdMaker(platformPath);
    for(int i = 0; i < 3; ++i)
        pthread_create(&pts[i],NULL,platformListener,(void*)&usfd);
    
    //while(availablePlatforms.size() != 3){}
    struct pollfd fds[3];
   
    for(int i = 0; i < 3; ++i){
        fds[i].fd = trainListener[i];
        fds[i].events = POLLIN;
        fds[i].revents = 0;
    }

    while(1){
        int ne = poll(fds,3,1000*5);
        if(!ne)continue;

        for(int i = 0; i < 3; ++i){
            if(fds[i].revents & POLLIN){
                while(availablePlatforms.size() == 0){}
                int platform = availablePlatforms.top();
                availablePlatforms.pop();
                sockaddr_un clien;
                socklen_t clienlen = sizeof(clien);
                int nusfd = accept(fds[i].fd,(sockaddr*)&clien,&clienlen);
                send_fd(platform,nusfd);
                break;
            }
        }
    }
    return 0;
}