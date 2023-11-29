#include "cn_header.h"

#define PORT 3080
#define SERVICE 3
#define CONTROL_BUFFER_SIZE CMSG_SPACE(sizeof(int))

char ADDR[3][10] = {
	"udsocket1",
	"udsocket2",
	"udsocket3"
};

struct msg_buf {
    long type ;
    char text[100] ;
} ;

struct sockaddr_un server_addr[3], client_addr[3] ;
struct sockaddr_in serv_i_addr ;

int fd[2], usfd[3], nusfd[3], sfd;
int addr_len[3], addrlen ;


struct iovec io_vec[1] ;
struct msghdr msg_header;
struct cmsghdr *recv_cmsg, *send_cmsg;

char control_buff[CONTROL_BUFFER_SIZE] ;
char buffer[1] = {'F'};

void print_connection(int sfd, int bind_resp, int listen_resp)
{
	printf("Entity\t|Response\n");
	printf("--------|--------\n");
	printf("socket\t| %d\nbind\t| %d\nlisten\t| %d\n",sfd,bind_resp,listen_resp);

}

void connect_init()
{
    serv_i_addr.sin_family = AF_INET ;
    serv_i_addr.sin_port = htons(PORT) ;
    serv_i_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
    addrlen = sizeof(server_addr) ;
    
    sfd = socket(AF_INET, SOCK_STREAM, 0) ;
    int bind_resp = bind(sfd, (struct sockaddr *)&serv_i_addr, addrlen) ;
    int listen_resp = listen(sfd, 2) ;
    print_connection(sfd, bind_resp, listen_resp);
}

void send_receive(int loc)
{
	send_cmsg = CMSG_FIRSTHDR(&msg_header) ;
	send_cmsg->cmsg_len = CMSG_LEN(sizeof(int)) ;
	send_cmsg->cmsg_level = SOL_SOCKET ;
	send_cmsg->cmsg_type = SCM_RIGHTS ;
	*((int *)CMSG_DATA(send_cmsg)) = fd[0] ;
	sendmsg(usfd[loc], &msg_header, 0) ;
	
	recvmsg(usfd[loc], &msg_header, 0);
	recv_cmsg = CMSG_FIRSTHDR(&msg_header) ;
	fd[loc] = *((int *)CMSG_DATA(recv_cmsg)) ;
}

int main() {
	
	int msg_ID = msgget((key_t)ftok("msg_header.txt", 'b'), 0777|IPC_CREAT) ;

	fd[0] = socket (AF_INET, SOCK_RAW, 2);	
	int opt=1;
	setsockopt(fd[0], IPPROTO_IP, SO_BROADCAST, &opt, sizeof(int));
	
	int i ;
	for(i=0; i<SERVICE; i++)
	{
		usfd[i] = socket(AF_UNIX, SOCK_STREAM, 0) ;

		bzero(&server_addr[i], sizeof(server_addr[i])) ;
		server_addr[i].sun_family = AF_UNIX ;
		strcpy(server_addr[i].sun_path, ADDR[i]);

		int bret = bind(usfd[i], (struct sockaddr *)&server_addr[i], sizeof(server_addr[i])) ;
		nusfd[i] = accept(usfd[i], (struct sockaddr *)&client_addr[i], &addr_len[i]) ;
		
		unlink(ADDR[i]);
	}

	io_vec[0].iov_base = buffer;
	io_vec[0].iov_len = 1 ;

	msg_header.msg_name = NULL ;
	msg_header.msg_namelen = 0 ;
	msg_header.msg_iov = io_vec ;
	msg_header.msg_iovlen = 1 ;
	msg_header.msg_control = control_buff ;
	msg_header.msg_controllen = CONTROL_BUFFER_SIZE ;

	for(i=0; i<SERVICE; i++) 
		send_receive(i);
	
	connect_init() ;
	struct msg_buf r_msg, s_msg;
	while(1) 
	{
		fd[1] = accept(sfd, (struct sockaddr *)&serv_i_addr, (socklen_t *)&addrlen) ;
		char message[1024] ;
		bzero(message, sizeof(message)) ;
		int resp_ret = recv(fd[i], message, 1024, 0) ;
		int num = atoi(message) ;
		for(i=0; i<SERVICE; i++) 
		{
			send_receive(i);
			int resp_ret = msgrcv(msg_ID, &r_msg, sizeof(r_msg), 2, 0) ;
			printf("response return =%d \nmessage [received]=%s\n", resp_ret, r_msg.text) ;
			s_msg.type = num ;
			strcpy(s_msg.text, r_msg.text) ;
			printf("\nmessage [sent]=%s\n", s_msg.text) ;
			msgsnd(msg_ID, &s_msg, sizeof(s_msg), 0) ;
		}
		
	}
	close(fd[0]) ; close(fd[1]) ;
	close(nusfd[0]) ; close(nusfd[1]) ; close(nusfd[2]) ;
	return 0 ;
}
