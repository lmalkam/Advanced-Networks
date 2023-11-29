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
#include<ctype.h>

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Enter protocal in arguments\n");
		return 0;
	}
	char va[100]={0};
	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1]));
	if(rsfd==-1){perror("socket");}

	int optval=1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL

	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

	unsigned int client_len=sizeof(client);

	char buff[]="request";
	if(sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client))<0){perror("socket");}
	char buf[1024];
	struct sockaddr_in serv;int servlen=sizeof(serv);
	char dad[]="";
	struct iphdr *ip;
	while(1){
		if(recvfrom(rsfd,buf,1024,0,(struct sockaddr *)&serv,(socklen_t *)&servlen)<0){perror("recv");}
		
		ip=(struct iphdr*)buf;
		bzero(dad,strlen(dad));
		strcat(dad,(buf+(ip->ihl)*4));
		if(isdigit(dad[0]))break;
	}
	int i;
	char num[5];
	int len = strlen(dad);
	printf("%s\n",dad);
	for(i=0;i<strlen(dad);i++){
		if(*(buf+(ip->ihl)*4+i)==' '){break;}
		num[i] = *(buf+(ip->ihl)*4+i);
	}
	num[i]='\0';

	int p = atoi(num);

	int x=0;
	while(x<p-1){
		bzero(buf,1024);
		if(recvfrom(rsfd,buf,1024,0,(struct sockaddr *)&serv,(socklen_t *)&servlen)<0){perror("recv");}
		struct iphdr *pp;
		pp=(struct iphdr*)buf;
		char ms[]="";
		strcat(ms,(buf+(pp->ihl)*4));
		if(isdigit(ms[0])){x++;printf("%s\n",(buf+(pp->ihl)*4) );}
	}
	close(rsfd);
	printf("enter sno:\n");
	int sno;
	scanf("%d",&sno);
	int port[sno];
	printf("Enter ports:\n");
	for(int i=0;i<sno;i++){
		scanf("%d",&port[i]);
	}

	int sfd[sno];

	for(int i=0;i<sno;i++)
	{
		sfd[i] = socket(AF_INET,SOCK_STREAM,0);		if(sfd[i]==-1){perror("socket()");}
	
		struct sockaddr_in for_addr;
		for_addr.sin_family = AF_INET;
		for_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		for_addr.sin_port = htons(port[i]); 

	    if(connect(sfd[i],(struct sockaddr *)&for_addr,sizeof(for_addr))<0)
	    {
	    	perror("connect()");
	    }
	}

	fd_set rfds;
	while(1)
	{
		FD_ZERO(&rfds);
		FD_SET(0,&rfds);
		for(int i=0;i<sno;i++)
		{
			FD_SET(sfd[i],&rfds);
		}

		int s = select(FD_SETSIZE,&rfds,NULL,NULL,NULL);
		if(s<0){perror("select():");}
		else
		{
			if(FD_ISSET(0,&rfds))
			{
				char r[1024];
				scanf("%s",r);
				write(sfd[r[0]-'1'],r,strlen(r)+1);
			}
			for(int i=0;i<sno;i++)
			{
				if(FD_ISSET(sfd[i],&rfds))
				{
					char w[1024];
					read(sfd[i],w,1024);
					printf("%s\n",w);
				}
			}
		}
	}

	return 0;
}