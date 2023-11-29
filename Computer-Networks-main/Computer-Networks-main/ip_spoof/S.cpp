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

#define IP "127.0.0.2"
#define blockedIP "127.0.0.3"

void print_ip_header(char* buff)
{
    struct sockaddr_in source,dest;
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr*)buff;
    iphdrlen =iph->ihl*4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
     
    printf("\n");
    printf("IP Header\n");
    printf("   |-IP Version        : %d\n",(unsigned int)iph->version);
    printf("   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    printf("   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    printf("   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    printf("   |-Identification    : %d\n",ntohs(iph->id));
    printf("   |-TTL      : %d\n",(unsigned int)iph->ttl);
    printf("   |-Protocol : %d\n",(unsigned int)iph->protocol);
    printf("   |-Checksum : %d\n",ntohs(iph->check));
    printf("   |-Source IP        : %s\n" , inet_ntoa(source.sin_addr) );
    printf("   |-Destination IP   : %s\n" , inet_ntoa(dest.sin_addr) );
}

int main(){

    int sfd = socket(AF_INET,SOCK_RAW,250);
    if(sfd < 0){
        perror("socket() : ");
        exit(-1);
    }
    int op = 1;
	setsockopt(sfd,IPPROTO_IP,IP_HDRINCL,&op,sizeof(op));

    sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(IP);

    if(bind(sfd,(struct sockaddr*)&serv,sizeof(serv)) < 0){
        perror("bind() : ");
        exit(-1);
    }
    cout << "SET at " << inet_ntoa(serv.sin_addr) << "\n";
    while(1){
        sockaddr_in clien;
        socklen_t clienlen = sizeof(clien);
        char buf[512];
        int res = recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&clien,&clienlen);
        cout << "yes\n";
        if(res < 0){
            perror("recvfrom() : ");
            exit(-1);
        }

        print_ip_header(buf);
        struct iphdr* ip_header;
        ip_header = (struct iphdr*)buf;
        sockaddr_in source;
        memset(&source, 0, sizeof(source));
        source.sin_addr.s_addr = ip_header->saddr;
        bool isBlocked = strcmp(blockedIP,inet_ntoa(source.sin_addr)) == 0;

        char resp[512];

        struct iphdr* replyH;
        replyH = (struct iphdr*) resp;
        replyH->ihl = 5;
        replyH->version = 4;
        replyH->tos = 0;
        replyH->ttl = 255;
        replyH->protocol = 250;
        replyH->check = 0;
        replyH->id = 12354;
        replyH->saddr = inet_addr(IP);
        replyH->daddr = ip_header->saddr;

        if(isBlocked){
            char rep[] = "Your IP Address is BLOCKED";
            strcpy(resp+sizeof(struct iphdr),rep);
            replyH->tot_len = sizeof(iphdr) + strlen(rep);
        }else{
            char rep[] = "*******************\n*\tWELCOME\t*\n*******************";
            strcpy(resp+sizeof(struct iphdr),rep);
            replyH->tot_len = sizeof(iphdr) + strlen(rep);
        }
        
        res = sendto(sfd,resp,sizeof(resp),0,(sockaddr*)&clien,clienlen);
        if(res < 0){
            perror("sendto() : ");
            exit(-1);
        }
    }
    return 0;
}