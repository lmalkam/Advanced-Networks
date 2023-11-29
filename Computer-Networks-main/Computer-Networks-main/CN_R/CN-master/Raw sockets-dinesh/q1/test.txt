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

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Enter protocal in arguments\n");
		return 0;
	}

	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1]));
	if(rsfd==-1){perror("socket");}
		
	
	struct sockaddr_in client;
	socklen_t clilen=sizeof(client);
	printf("receive\n");
	while(1)
	{
		char buf[1024];
		if(recvfrom(rsfd,buf,1024,0,(struct sockaddr *)&client,(socklen_t *)&clilen)<0){perror("recv");}
	
		struct iphdr *ip;
	  	ip=(struct iphdr*)buf;
		printf("%s\n",(buf+(ip->ihl)*4));

	}
	
	return 0;
}