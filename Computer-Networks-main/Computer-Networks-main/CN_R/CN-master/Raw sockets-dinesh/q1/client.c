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
int main(int argc,char *argv[])
{
	struct sockaddr_in loc_addr,for_addr;
	if(argc<3)
	{
		printf("Enter port numnber and address number\n");
		return 0;
	}
	

	int sfd = socket(AF_INET,SOCK_STREAM,0);		if(sfd==-1){perror("socket()");}
	
	
	for_addr.sin_family = AF_INET;
	for_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	for_addr.sin_port = htons(atoi(argv[1])); 

    if(connect(sfd,(struct sockaddr *)&for_addr,sizeof(for_addr))<0)
    {
    	perror("connect()");
    }

    printf("Enter address number:\n");
    char num[10],name[100];
    scanf("%s",num);
    printf("Enter file name:\n");
    scanf("%s",name);
    strcat(num,"|");
    strcat(num,name);
    strcat(num,"|");

    write(sfd,num,strlen(num)+1);

    printf("%s\n",argv[2] );
    int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[2]));
    if(rsfd<0){perror("socket");}

    struct sockaddr_in client;
    client.sin_family=AF_INET;
    client.sin_addr.s_addr=INADDR_ANY;

    if(bind(rsfd,(struct sockaddr *)&client,sizeof(client))<0){perror("bind()");}
   // while(1)
    {
        int loc_length = sizeof(loc_addr);
        if(connect(rsfd,(struct sockaddr *)&client,sizeof(client))<0)
            {
                perror("connect()");
            }
        char buf[1024];
        while(read(rsfd,buf,1024)>0){
    
        struct iphdr *ip;
        ip=(struct iphdr*)buf;
        printf("%s",(buf+(ip->ihl)*4));
        bzero(buf,1024);
    }
    }


    close(sfd);
    return 0;
}