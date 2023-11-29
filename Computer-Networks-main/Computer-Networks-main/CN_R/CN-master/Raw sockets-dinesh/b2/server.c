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
void *runner(void *p)
{
	struct sockaddr_in client;
	socklen_t clilen=sizeof(client);
	int rsfd = *(int *)p;
	while(1)
	{
		char buf[1024];
		if(recvfrom(rsfd,buf,1024,0,(struct sockaddr *)&client,(socklen_t *)&clilen)<0){perror("recv");}

		struct iphdr *ip;
	  	ip=(struct iphdr*)buf;
		printf("%s\n",(buf+(ip->ihl)*4));
	}
}
int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Enter protocal in arguments\n");
		return 0;
	}

	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1]));
	if(rsfd==-1){perror("socket");}

	int optval=1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL

	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

		
	client.sin_addr.s_addr=INADDR_ANY;

	unsigned int client_len=sizeof(client);
	
	

	pthread_t tid; 
	pthread_create(&tid,NULL,runner,(void *)&rsfd);

	printf("send and Enter msg\n");
	while(1)
	{
		char buff[100];
		scanf("%s",buff);
		if(sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client))<0){perror("socket");}

	}
	
	return 0;
}