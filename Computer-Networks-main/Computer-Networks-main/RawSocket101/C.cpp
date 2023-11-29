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


int main(){

    int rsfd = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
    if(rsfd < 1){
        perror("socket ");
    }
    struct sockaddr_in serv;
    socklen_t servlen = sizeof(serv);

    char buff[100];
    recvfrom(rsfd,buff,sizeof(buff),0,(sockaddr*)&serv,&servlen);

    struct iphdr *ip;
    ip=(struct iphdr*)buff;
	cout<<(buff+(ip->ihl)*0)<<endl;
    cout<<(buff+(ip->ihl)*1)<<endl;
    cout<<(buff+(ip->ihl)*2)<<endl;
    cout<<(buff+(ip->ihl)*3)<<endl;
    cout<<(buff+(ip->ihl)*4)<<endl;


    return 0;
}