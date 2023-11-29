#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/select.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <sys/un.h>
#include <pcap.h>
#include <pthread.h>

struct Linesh_TCP
{
	uint16_t source;
	uint16_t dest;
	uint8_t connect_req;
	uint8_t connect_acc;
	uint8_t final_acknow;
	uint8_t data_flg;
	uint16_t seq;
	uint16_t rec_seq;
};

/// PORTS FOR COMMUNICATION (7777 -> CLIENT) AND (3535 -> SERVER)
int PORT1 = 7777;
int PORT2 = 3535;

int linesh_connect(int sfd,struct sockaddr* addr,ssize_t sz)
{
	struct Linesh_TCP *request;int sz1;
	while(1)
	{
		request = (struct Linesh_TCP*)malloc(sizeof(struct Linesh_TCP));
		memset(request,0,sizeof(request));
		request->source = PORT1;
		request->dest = PORT2;
		request->connect_req = 1;
		request->seq = 5;
		request->rec_seq = 0;
		request->final_acknow = 0;
		sendto(sfd,request,sizeof(request),0,addr,sz);
		char buffer[1000];
		if(recvfrom(sfd,buffer,1000,0,addr,&sz1)>=0){
			printf("Conection was accepted by the server\n");
        }
		struct iphdr* ip;
		ip = (struct iphdr*)buffer;
		request = (struct Linesh_TCP*)(buffer+(ip->ihl*4));
        printf("Connection Accepted Flag : %d\n",request->connect_acc);
		if(request->connect_acc==1)
			break;
	}

	printf("Continue \n");
	request = (struct Linesh_TCP*)malloc(sizeof(struct Linesh_TCP));
	memset(request,0,sizeof(request));
	request->source = PORT1;
	request->dest = PORT2;
	request->connect_req = 0;
	request->seq = 5;
	request->rec_seq = 0;
	request->final_acknow = 1;
	if(sendto(sfd,request,sizeof(request),0,addr,sz1)>=0)
    {
		printf("Sent the Final acknowledge Flag\n");
    }
	return 1;
}

int custom_msg(int sfd,char *buffer,int bufsiz,struct sockaddr* addr,ssize_t sz)
{
	char msg[1000];
	struct Linesh_TCP *buff;
	buff = (struct Linesh_TCP*)(msg);
	memset(buff,0,sizeof(buff));

    //Defining the fields
	buff->source = PORT1;
	buff->dest = PORT2;
	buff->connect_req = 0;
	buff->connect_acc = 0;
	buff->seq = 0;
	buff->rec_seq = 0;
	buff->final_acknow =0;
	buff->data_flg = 1;
	memcpy(msg+sizeof(*buff),buffer,bufsiz);
	sendto(sfd,msg,1000,0,addr,sz);
}


int main()
{

    int sfd = socket(AF_INET,SOCK_RAW,253);
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.2");
	struct sockaddr_in addr1;
	memset(&addr1,0,sizeof(addr1));
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = inet_addr("127.0.0.7");
	bind(sfd,(struct sockaddr*)&addr,sizeof(addr));
	linesh_connect(sfd,(struct sockaddr*)&addr1,sizeof(addr1));

	char buffer[100] = "Linesh 21CSB0A35";
	custom_msg(sfd,buffer,strlen(buffer),(struct sockaddr*)&addr1,sizeof(addr1));
	return 0;

}