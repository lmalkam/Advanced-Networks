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
	if(argc!=2)
	{
		printf("Enter port numnber\n");
		return 0;
	}

	int sfd = socket(AF_INET,SOCK_STREAM,0);

	if(sfd==-1)
	{
		perror("socket()");
	}

	struct sockaddr_in loc_addr,for_addr;
	loc_addr.sin_family = AF_INET;
	loc_addr.sin_addr.s_addr = INADDR_ANY; 
    loc_addr.sin_port = htons(atoi(argv[1])); 

    if(bind(sfd,(struct sockaddr *)&loc_addr,sizeof(loc_addr))<0)				{perror("bind()");}

    if (listen(sfd,4) < 0) 														{perror("listen"); } 
    

    int for_length = sizeof(for_addr);
    
    while(1)
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

            char msg[1024];
            if(read(nsfd,msg,1024)<0)               {perror("read()");}
            printf("%s\n",msg);
            sleep(1);
            char num[10],name[10];
            int i=0,j=0;
            while(msg[i]!='|')
            {
                num[j++] = msg[i++];
            }
            num[i++]='\0';
            j=0;
            while(msg[i]!='|')
            {
                name[j++] = msg[i++];
            }
            name[j]='\0';
            int rsfd=socket(AF_INET,SOCK_RAW,atoi(num));
            if(rsfd<0){perror("socket");}

            int optval=1;
            setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL

            struct sockaddr_in loc,serv;
            serv.sin_family=AF_INET;
            serv.sin_addr.s_addr=inet_addr("127.0.0.1");
            serv.sin_addr.s_addr=INADDR_ANY;

            if(connect(rsfd,(struct sockaddr *)&serv,sizeof(serv))<0)
            {
                perror("connect()");
            }

            int fd = open(name,O_RDONLY);
            dup2(fd,0);
            dup2(rsfd,1);
            char *pas[]={"kjksdn",NULL};
            execvp("./s2",pas);
        }
        
       
    }
   
    return 0;


}
