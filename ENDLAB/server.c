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

int seq =0;

int custom_receive(int sfd,char *buffer,int bufsiz,struct sockaddr* addr,int *sz)
{
	char buffer1[1000];
	
	recvfrom(sfd,buffer1,1000,0,addr,sz);
	struct Linesh_TCP *buff;
	struct iphdr* ip;
	ip = (struct iphdr*)(buffer1);
	buff = (struct Linesh_TCP*)(buffer1+(ip->ihl*4));
	memset(buff,0,sizeof(buff));
	seq +=buff->seq;
    
	memcpy(buffer,buffer1+(ip->ihl*4)+sizeof(*buff),bufsiz);
}

int linesh_accept(int sfd,struct sockaddr* addr,int *sz)
{
	struct Linesh_TCP *buff;
	char buffer[1000];
	recvfrom(sfd,buffer,1000,0,addr,sz);
	struct iphdr* ip;
	ip = (struct iphdr*)buffer;
	buff = (struct Linesh_TCP*)(buffer+(ip->ihl*4));
	printf("Connection Req Flag from Client: %d\n",buff->connect_req);
	buff = (struct Linesh_TCP*)malloc(sizeof(struct Linesh_TCP));
	memset(buff,0,sizeof(buff));
	buff->source = PORT1;
	buff->dest = PORT2;
	buff->connect_req = 0;
	buff->connect_acc = 1;
	buff->seq = 0;
	buff->rec_seq = 0;
	buff->final_acknow = 0;

	if(sendto(sfd,buff,sizeof(buff),0,addr,*sz)>=0){
        printf("Reply was sent to client \n");
    }
		
	memset(buffer,0,1000);
	recvfrom(sfd,buffer,1000,0,addr,sz);
    ip = (struct iphdr*)buffer;
	buff = (struct Linesh_TCP*)(buffer+(ip->ihl*4));
    printf("Final Acknowledge number flag : %d\n",buff->final_acknow);
	return 1;
}


int main()
{

    int sfd = socket(AF_INET,SOCK_RAW,253);
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.7");
	struct sockaddr_in addr1;int sz = sizeof(addr1);
	bind(sfd,(struct sockaddr*)&addr,sizeof(addr));
	
	linesh_accept(sfd,(struct sockaddr*)&addr1,&sz);
	
	char buffer[100];
	int sz1;
	custom_receive(sfd,buffer,100,(struct sockaddr*)&addr1,&sz1);
	printf("Received: %s\n",buffer);
	return 0;


}