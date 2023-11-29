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
    int optval = 1;
    int res = setsockopt(rsfd,IPPROTO_IP,SO_BROADCAST,&optval,sizeof(int));
    if(res < 0){
        perror("sockopt :");
    }
    struct sockaddr_in clien;
    clien.sin_family = AF_INET;
    clien.sin_addr.s_addr = INADDR_ANY;

    char buff[] = "Here we go again";

    sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&clien,sizeof(clien));
    perror("sendto ");


    return 0;
}