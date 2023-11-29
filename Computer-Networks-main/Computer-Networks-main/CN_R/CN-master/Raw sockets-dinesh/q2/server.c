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
	if(argc!=3)
	{
		printf("Enter port and protocol numnber\n");
		return 0;
	}
	key_t key;
	key=ftok("shmfile",65);
	int msgid=shmget(key,sizeof(int),0666|IPC_CREAT);
	int *p=(int*)shmat(msgid,0,0);
	int i=*p;i++;*p=i;
	sleep(1);
	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[2]));
	if(rsfd==-1){perror("raw socket");}
	printf("rsfd%d\n",rsfd );

	int optval=1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL




	int sfd = socket(AF_INET,SOCK_STREAM,0);
	printf("sfd: %d\n",sfd );

	if(sfd==-1)
	{
		perror("socket()");
	}


	struct sockaddr_in loc_addr,for_addr,client;
	loc_addr.sin_family = AF_INET;
	loc_addr.sin_addr.s_addr = INADDR_ANY; 
    loc_addr.sin_port = htons(atoi(argv[1])); 

	client.sin_family=AF_INET;
	client.sin_addr.s_addr=INADDR_ANY;
	unsigned int client_len=sizeof(client);
	int opt=1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));
    if(bind(sfd,(struct sockaddr *)&loc_addr,sizeof(loc_addr))<0)				{perror("bind()");}

    if (listen(sfd,4) < 0) 														{perror("listen"); } 
    

    int for_length = sizeof(for_addr);
    fd_set rfds;

    while(1)
    {
    	FD_ZERO(&rfds);
    	FD_SET(rsfd,&rfds);
    	FD_SET(sfd,&rfds);

    	int s = select(FD_SETSIZE,&rfds,NULL,NULL,NULL);
    	if(s<0)
    	{
    		perror("select()");
    	}
    	else 
    	{
    		if(FD_ISSET(sfd,&rfds))
    		{
    			int nsfd = accept(sfd,(struct sockaddr *)&for_addr,&for_length);            if(nsfd<0){perror("accept()"); }

		        int c = fork();
		        if(c)
		        {
		            close(nsfd);
		        }   
		        
		        else
		        {

		            close(sfd);

		            while(1)
		            {
		                char msg[1024];
		                if(read(nsfd,msg,1024)<0)               {perror("read()");}
		               	strcat(msg,":");
		               	strcat(msg,argv[1]);
		                printf("%s\n",msg);
		                if(write(nsfd,msg,strlen(msg)+1)<0)     {perror("write()");}
		            }
		             close(nsfd);
		            close(sfd);
		        }
	    	}
    		else if(FD_ISSET(rsfd,&rfds))
    		{
    			char buf[1024];
    			if(recvfrom(rsfd,buf,1024,0,(struct sockaddr *)&client,(socklen_t *)&client_len)<0){perror("recv");}
    			struct iphdr *ip;
				ip=(struct iphdr*)buf;
				char ms[]="";
				strcat(ms,(buf+(ip->ihl)*4));
				
				if(ms[0]=='r' && ms[1]=='e'){
					printf("%s\n",ms );
	    			char msg[100];
	    			sprintf(msg,"%d",*p);
	    			strcat(msg," server:");
	    			strcat(msg,argv[1]);
	    			strcat(msg," resend");

	    			if(sendto(rsfd,msg,strlen(msg)+1,0,(struct sockaddr*)&client,sizeof(client))<0){perror("socket");}
	    		}

    		}
    	}
        
        
       
    }
   
    return 0;
}