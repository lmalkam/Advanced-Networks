#include "cn_header.h"

#define ACTION 2
#define ADDR "udsocket1"
#define SIZE 1024
#define CONTROL_BUFFER_SIZE CMSG_SPACE(sizeof(int))
#define RESULT_BUFFER_SIZE 50

struct msg_buf {
    long type ;
    char text[100] ;
} ;

struct sockaddr_un server_addr, client_addr ;
struct sockaddr_in client ;

struct iovec io_vec[1] ;
struct msghdr msg_header ;
struct cmsghdr *recv_cmsg[2] , *send_cmsg[2] ;
char control_buff[CONTROL_BUFFER_SIZE] ;
int usfd, fd[2], addr_len ;
char buffer[1] = {'F'} ;


void rec_utils(int i) {
	char message[SIZE] ;
	bzero(message, sizeof(message)) ;
	int ret = recv(fd[i], message, SIZE, 0) ;
	char result[RESULT_BUFFER_SIZE] ;
	if(strlen(message) != 10) strcpy(result, "Not Verified") ;
	else strcpy(result, "Verified") ;
	
	int msgid ;
	key_t key ;
	key = ftok("msg.txt", 'b') ;
	msgid = msgget(key, 0777) ;
	struct msg_buf send_msg ;
	send_msg.type = 1 ;
	strcpy(send_msg.text, result) ;
	printf("message[sent] = %s\n", send_msg.text) ;
	msgsnd(msgid, &send_msg, sizeof(send_msg), 0) ;
}

void receive_send_fd(int i)
{
	// receiving
	recvmsg(usfd, &msg_header, 0) ;
	recv_cmsg[i] = CMSG_FIRSTHDR(&msg_header) ;
	fd[i] = *((int *)CMSG_DATA(recv_cmsg[i])) ;

	if (!i)
	{
		client.sin_family=AF_INET;
		client.sin_addr.s_addr=INADDR_ANY;
		char buffer[] = "PAN" ;
		socklen_t client_len=sizeof(client);
		sendto(fd[0],buffer,SIZE,0,(struct sockaddr*)&client,(socklen_t)client_len);
	}
	else
		rec_utils(i);

	// sending
	send_cmsg[i] = CMSG_FIRSTHDR(&msg_header) ;
	send_cmsg[i]->cmsg_len = CMSG_LEN(sizeof(int)) ;
	send_cmsg[i]->cmsg_level = SOL_SOCKET ;
	send_cmsg[i]->cmsg_type = SCM_RIGHTS ;
	*((int *)CMSG_DATA(send_cmsg[i])) = fd[i] ; //printf("sending %d\n", fdPass) ;

	sendmsg(usfd, &msg_header, 0) ;
}

int main() {
	// connecting USFD
	usfd = socket(AF_UNIX, SOCK_STREAM, 0) ; 
	bzero(&server_addr, sizeof(server_addr)) ;
	server_addr.sun_family = AF_UNIX ;
	strcpy(server_addr.sun_path, ADDR) ;
	connect(usfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ; 
	
	// setting up USD
	io_vec[0].iov_base = buffer ;
	io_vec[0].iov_len = 1 ;

	msg_header.msg_name = NULL ;
	msg_header.msg_namelen = 0 ;
	msg_header.msg_iov = io_vec ;
	msg_header.msg_iovlen = 1 ;
	msg_header.msg_control = control_buff ;
	msg_header.msg_controllen = CONTROL_BUFFER_SIZE ;

	int opt = 1 ;
	setsockopt(fd[0], IPPROTO_IP, SO_BROADCAST, &opt, sizeof(int));
	
	for(int i = 0 ; i < ACTION;i++)
	 	receive_send_fd(i);
	
	return 0 ;
}
