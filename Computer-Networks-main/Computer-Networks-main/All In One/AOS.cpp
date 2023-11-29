#include<bits/stdc++.h>
#include<netinet/in.h>
#include <sys/un.h>
#include<sys/socket.h>
#include<sys/poll.h>
using namespace std;

unordered_map<int,int> usfdMapper;

int sfdMaker(int port){
    struct sockaddr_in aos;
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
	aos.sin_family = AF_INET;
	aos.sin_addr.s_addr = INADDR_ANY;
	aos.sin_port = htons(port);
	if(bind(sfd,(struct sockaddr*) &aos, sizeof(aos)) < 0){
		perror("bind failed");
		return 0;
	}
	if(listen(sfd,3) < 0){
		perror("listen failed");
		return 0;
	}
    return sfd;
}

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

int main(){

    for(int i = 8081; i <= 8084; ++i){
        int usfd = socket(AF_UNIX,SOCK_STREAM,0);

        struct sockaddr_un addr;
        addr.sun_family = AF_UNIX;
        char port[20];
        sprintf(port,"%d",i);
        strcpy(addr.sun_path,port);

        if(connect(usfd,(struct sockaddr *)&addr,sizeof(addr))==-1)perror("\n connect "); 
        else {
            cout << "Server " << i << " Connected" << endl;
            usfdMapper[i-8081] = usfd;
        }
    }

    struct pollfd pfds[4];

    for(int i = 0; i < 4; ++i){
        pfds[i].fd = sfdMaker(8001+i);
        pfds[i].events = 0x001;
        pfds[i].revents = 0;
    }

    
    while(1){
        int ne = poll(pfds,4,1000*5);
        if(!ne)continue;

        for(int i = 0; i < 4; ++i){
            if(pfds[i].revents & 0x001){
                cout << i << endl;
                if(i != 3){
                    struct sockaddr_in clien;
                    int clienlen = sizeof(clien);
                    int nsfd = accept(pfds[i].fd,(struct sockaddr*)&clien,(socklen_t*)&clienlen);
                    cout << usfdMapper[i] << " " << nsfd << endl;
                    send_fd(usfdMapper[i],nsfd);
                    cout << "FD Sent\n";
                }else{
                    send_fd(usfdMapper[i],pfds[i].fd);
                    close(pfds[i].fd);
                }
            }
        }
    }
    return 0;
}


