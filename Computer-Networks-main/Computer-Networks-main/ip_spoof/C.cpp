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

#define IP "127.0.0.3"

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

    sockaddr_in own;
    own.sin_family = AF_INET;
    own.sin_addr.s_addr = inet_addr(IP);

    if(bind(sfd,(struct sockaddr*)&own,sizeof(own)) < 0){
        perror("bind() : ");
        exit(-1);
    }

    char buf[512];
    struct iphdr *ip_header;
    ip_header = (struct iphdr*)buf;
    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = sizeof(struct iphdr);
    ip_header->id = htons(12345);
    ip_header->frag_off = 0;
    ip_header->ttl = 255;
    ip_header->protocol = 250;
    ip_header->check = 0;
    ip_header->saddr = inet_addr(IP);


    while(1){
        int c;
        cout << "Use VPN ? : ";
        cin >> c;
        cout << c << endl;
        sockaddr_in serv;
        memset(&serv,0,sizeof(serv));
        serv.sin_family = AF_INET;
        if(c){
            serv.sin_addr.s_addr = inet_addr("127.0.0.6");
            ip_header->daddr = inet_addr("127.0.0.6");
        }else{
            serv.sin_addr.s_addr = inet_addr("127.0.0.2");
            ip_header->daddr = inet_addr("127.0.0.2");
        }
        int res = sendto(sfd,buf,sizeof(buf),0,(struct sockaddr*)&serv,sizeof(serv));
        if(res < 0){
            perror("sendto() : ");
            exit(-1);
        }
        cout << "SENT to " << inet_ntoa(serv.sin_addr) << endl;

        char resp[512];
        sockaddr_in clien;
        socklen_t clienlen = sizeof(clien);
        res = recvfrom(sfd,resp,sizeof(resp),0,(struct sockaddr*)&clien,&clienlen);
        if(res < 0){
            perror("recvfrom() : ");
            exit(-1);
        }
        printf("Payload:\n %s\n", resp + sizeof(struct iphdr));
        print_ip_header(resp);
    }    
    return 0;
}