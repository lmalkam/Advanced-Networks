#include<bits/stdc++.h>
#include<netinet/in.h>
#include <sys/un.h>
#include<sys/socket.h>
#include<sys/poll.h>
using namespace std;

#define portPath "8084"

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


int main(){
    int usfd = usfdMaker();
    
    sockaddr_un clien;
    int clienlen = sizeof(clien);

    int nusfd = accept(usfd,(struct sockaddr*)&clien,(socklen_t*)&clienlen);
    if(nusfd < 0){
        perror("nusfd failed");
        exit(0);
    }
    cout << "Accepted\n";

    int sfd = recv_fd(nusfd);
    int nsfd;
    char message[] = "Hello From S4";
    while((nsfd = accept(sfd,(sockaddr*)&clien,(socklen_t*)&clienlen)) > 0){
        send(nsfd,message,strlen(message)+1,0);
    }

    return 0;
}


