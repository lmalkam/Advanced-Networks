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

vector<int> nsfds[100];

int sfdMaker(int port){
    struct sockaddr_in AT;
    int sfd;
	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("sfd fail");
		return 0;
	}
	int opt = 1;
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        	perror("setsockopt");
        	return 0;
    }
	AT.sin_family = AF_INET;
	AT.sin_addr.s_addr = INADDR_ANY;
	AT.sin_port = htons(port);
	if(bind(sfd,(struct sockaddr*) &AT, sizeof(AT)) < 0){
		perror("bind failed");
		return 0;
	}
	if(listen(sfd,3) < 0){
		perror("listen failed");
		return 0;
	}
    return sfd;
}

void* acep(void* args){
    pair<int,int> p = *(pair<int,int>*)args;
    while(1){
        sockaddr_in clien;
        socklen_t cliensize = sizeof(clien);
        int nsfd = accept(p.second,(sockaddr*)&clien,&cliensize);
        cout << "ACCEPTED " << p.first << endl;
        nsfds[p.first].push_back(nsfd);
    }
    return nullptr;
}

void* serv(void* args){
    int i = *(int*)args;

    int sfd = sfdMaker(8000+i);

    pthread_t pt;
    pair<int,int> p = {i,sfd};
    pthread_create(&pt,NULL,acep,(void*)&p);

    char message[1000];
    sprintf(message,"%s %d", "Hey There ", i);
    cout << message << endl;
    while(1){
        for(auto nsfd : nsfds[i]){
            cout << "WHILE : " << nsfd << endl;
            send(nsfd,message,strlen(message)+1,0);
        }
        sleep(5);
    }
}

int main(){

    pthread_t pt[12];

    for(int i = 0; i < 12; ++i){
        int* fix = (int*)malloc(sizeof(int));
        *fix = i+1;
        pthread_create(&pt[i],NULL,serv,fix);
    }
    for(int i = 0; i < 12; ++i){
        pthread_join(pt[i],NULL);
    }

    return 0;
}