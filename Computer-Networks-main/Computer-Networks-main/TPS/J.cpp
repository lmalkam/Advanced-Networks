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

int usfdMaker(char* portS){
    sockaddr_un serv;
    int usfd = socket(AF_UNIX,SOCK_STREAM,0);
    serv.sun_family = AF_UNIX;
    strcpy(serv.sun_path,portS);
    unlink(portS);
    if(bind(usfd,(sockaddr*)&serv,sizeof(serv)) < 0){
        perror("bind fail ");
        exit(0);
    }
    if(listen(usfd,5) < 0){
        perror("listen fail ");
        exit(0);
    }
    return usfd;
}

unordered_map<int,candidate> mapper;
unordered_map<int,int> scores;

int main(){

    int usfd = usfdMaker("JP");

    for(int i = 0; i < 3*5; ++i){
        int nusfd;
        sockaddr_un clien;
        socklen_t cliensize = sizeof(clien);
        if((nusfd = accept(usfd,(sockaddr*)&clien,&cliensize)) <= 0)perror("Nusfd failed ");
        else{
            candidate c;
            int score;
            char buff[sizeof(candidate)];
            read(nusfd,buff,sizeof(buff));
            memcpy(&c,buff,sizeof(candidate));
            read(nusfd,&score,sizeof(int));
            cout << c.name << " " << score << endl;
            if(mapper.find(c.roll) == mapper.end()){
                mapper[c.roll] = c;
                scores[c.roll] = score;
            }else{
                scores[c.roll] += score;
            }
        }
    }
    for(auto pp : mapper){
        cout << pp.second.name << " " << scores[pp.first] << endl;
    }
    return 0;
}