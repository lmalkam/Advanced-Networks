#include<bits/stdc++.h>
#include<netinet/in.h>
#include <sys/un.h>
#include<sys/socket.h>

using namespace std;

#define portC 8000
#define portS "mySocket"

struct sockaddr_in iss;
struct sockaddr_un serv;

int sfdMaker(){
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
	iss.sin_family = AF_INET;
	iss.sin_addr.s_addr = INADDR_ANY;
	iss.sin_port = htons(portC);
	if(bind(sfd,(struct sockaddr*) &iss, sizeof(iss)) < 0){
		perror("bind failed");
		return 0;
	}
	if(listen(sfd,3) < 0){
		perror("listen failed");
		return 0;
	}
    return sfd;
}

int usfdMaker(){
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

void* serviceReader(void* args){
    int nusfd = *(int*)args;
    cout << nusfd << endl;
    char message[1024];
    while(read(nusfd,message,sizeof(message))){
        char* finale = new char[2000];
        strcat(finale,"Message on nusfd ");
        strcat(finale, to_string(nusfd).c_str());
        strcat(finale," : ");
        strcat(finale,message);
        cout << finale << endl;
        memset(finale,'\0',sizeof(message));
        // delete[] finale;
        // cout << message << endl;
    }
    return NULL;
}

int main(){
    
    int sfd = sfdMaker();
    int usfd = usfdMaker();

    int nsfd; 

    struct sockaddr_in clien;
    int cliensize = sizeof(clien);

    char serviceName[] = {'.','/','S','\0'};

    while((nsfd = accept(sfd,(struct sockaddr*)&clien, (socklen_t*)&cliensize)) > 0){
        cout << "A Client Got Accepted\n";
        int c = fork();
        if(!c){
            close(sfd);
            close(usfd);
            execv(serviceName,NULL);
        }else{
            sockaddr_un clienn;
            socklen_t clienlen = sizeof(clienn);
            int nusfd;
            if((nusfd = accept(usfd,(sockaddr*)&clienn,&clienlen)) < 1){
                perror("Nusfd fail : ");
                exit(0);
            }
            int* fixnusfd = (int*)malloc(sizeof(int));
            *fixnusfd = nusfd;
            send_fd(nusfd,nsfd);
            close(nsfd);
            pthread_t pt;
            pthread_create(&pt,NULL,serviceReader,(void*)fixnusfd);
        }
    }
    return 0;
}