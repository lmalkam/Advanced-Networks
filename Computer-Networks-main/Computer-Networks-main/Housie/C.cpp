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

int numRecieved; 
int sfd;
int gameOver = 0;
int randInt;



void* handler(void* args){
    int* cp = (int*)args;
    for(int i = 0; i < 4; ++i)
            cout << cp[i] << " ";
    int rem = 4;
    cout << "a\n";
    int usfd = socket(AF_UNIX,SOCK_STREAM,0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    char sp[100];
    sprintf(sp,"%d",randInt);
    strcpy(addr.sun_path,sp);
    if(connect(usfd,(struct sockaddr *)&addr,sizeof(addr))==-1)perror("\n connect ");

    while(1){
        int num;
        read(usfd,&num,sizeof(int));
        printf("%d\n",num);
        if(num == -1)break;
        for(int i = 0; i < 4; ++i){
            if(cp[i] == num)rem--;
        }
        printf("Remaining %d", rem);
        if(rem == 0){
            int done = 1;
            send(sfd,&done,sizeof(done),0);
            read(sfd,&done,sizeof(int));
            if(done){
                cout << "I WON\n";
            }
            gameOver = 1;
            break;
        }
    }
}

int usfdMaker(char* socketPath){
    int usfd = socket(AF_UNIX,SOCK_STREAM,0);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path,socketPath);

    unlink(socketPath);
    if(bind(usfd,(sockaddr*)&addr,sizeof(addr)) < 0){
        perror("bind fail ");
        exit(0);
    }
    if(listen(usfd,5) < 0){
        perror("listen fail ");
        exit(0);
    }
    return usfd;
}

int main(int argc, char* argv[]){
	srand(time(0));
    randInt = rand();

    char sp[100];
    sprintf(sp,"%d",randInt);
    int usfd = usfdMaker(sp);

    int tc = atoi(argv[1]);
    struct sockaddr_in address;
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("sfd fail");
		exit(0);
	}



	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8000);
	
	if(connect(sfd,(struct sockaddr*)&address,sizeof(address)) < 0){
		perror("connect failed");
		exit(0);
	}

    send(sfd,&tc,sizeof(tc),0);

    for(int i = 0; i < tc; ++i){
        int c[4];
        read(sfd,c,4*sizeof(int));
        int* fix = (int*)malloc(4*sizeof(int));
        for(int i = 0; i < 4; ++i)
            fix[i] = c[i];
        pthread_t pt;
        pthread_create(&pt,NULL,handler,fix);
    }
    vector<int> nusfds;
    for(int i = 0; i < tc; ++i){
        int nusfd;
        sockaddr_un clien;
        socklen_t cliensize = sizeof(clien);
        nusfd = accept(usfd,(sockaddr*)&clien,&cliensize);
        nusfds.push_back(nusfd);
    }
    cout << nusfds.size() << endl;
    while(!gameOver){
        int num; 
        read(sfd,&num,sizeof(int)); 
        for(auto nusfd : nusfds){
            send(nusfd,&num,sizeof(int),0);
        }
        if(num == -1)break;
    }
    cout << "GG\n";
    return 0;
}