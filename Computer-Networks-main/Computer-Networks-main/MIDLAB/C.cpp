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

int AsfdMaker(int port){
    int sfd;
    sockaddr_in address;
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("sfd fail");
		exit(0);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
	if(connect(sfd,(struct sockaddr*)&address,sizeof(address)) < 0){
		perror("connect failed");
		exit(0);
	}
    cout << port << " COnnected\n";
    return sfd;
}


int main(int argc, char* argv[]){

    int port;
    if(argv[1][0] == 'M')port = 8001;
    else port = 8002;
    cout << port << endl;
    
    int sfd = AsfdMaker(port);

    int avail;
    read(sfd,&avail,sizeof(int));

    if(avail){
        cout << "Location : ";
        char location[100];
        scanf("%s",location);
        send(sfd,location,strlen(location)+1,0);
        char resp[1000];
        read(sfd,resp,sizeof(resp));
        cout << resp << endl;
    }else{
        cout << "No available\n";
    }


    return 0;
}