#include<bits/stdc++.h>
#include<sys/poll.h>
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
#include<sys/ipc.h>
#include<sys/msg.h>
#include<iostream>
#include<semaphore.h>
#include<cctype>
#include<sys/stat.h>
using namespace std;

struct candidate{
    char name[100];
    int roll;
    int done[3];
};

int main(){

    candidate c;
    cin >> c.name;
    cin >> c.roll;
    for(int i = 0; i < 3; ++i)c.done[i] = false;

    struct sockaddr_in address;
	
	int sfd;
	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("sfd fail");
		return 0;
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8000);
	
	if(connect(sfd,(struct sockaddr*)&address,sizeof(address)) < 0){
		perror("connect failed");
		return 0;
	}

    char buff[sizeof(candidate)];
    memcpy(buff,&c,sizeof(candidate));
    write(sfd,buff,sizeof(buff));
    int qualpassed;
    read(sfd,&qualpassed,sizeof(int));
    if(qualpassed == 0){
        cout << "qualfail\n";
        return 0;
    }
    for(int i = 0; i < 3; ++i){
        cout << "Waiting for interview " << i+1 << endl;
        char message[1024];
        read(sfd,message,sizeof(message));
        cout << message << endl;
        char resp[1024];
        scanf("%s",resp);
        // cin.getline(resp,sizeof(resp));
        send(sfd,resp,strlen(resp)+1,0);
    }

    return 0;
}