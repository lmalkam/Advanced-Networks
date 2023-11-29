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

int main()

{
	char msg[]="21|test.txt|";
	char num[10],name[10];
            int i=0,j=0;
            while(msg[i]!='|')
            {

                num[j++] = msg[i++];
                printf("%s\n",num);
            }
           // num[i++]='\0';
            printf("%s\n",num);
            //printf("%d\n",atoi(num));
            j=0;
            while(msg[i]!='|')
            {
                name[j++] = msg[i++];
            }
            name[j]='\0';
            printf("%s\n",num);
            printf("%d\n",atoi(num));
}