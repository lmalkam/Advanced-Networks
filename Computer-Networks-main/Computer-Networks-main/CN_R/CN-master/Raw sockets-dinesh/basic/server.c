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
	if(rsfd<0){perror("socket");}

	int optval=1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL

	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

		
	client.sin_addr.s_addr=INADDR_ANY;

	unsigned int client_len=sizeof(client);
	printf("sending\n");
	while(1)
	{
		char buff[1024];
		scanf("%s",buff);
		if(sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client))<0){perror("socket");}
  	
	}
  	
	
	return 0;
}