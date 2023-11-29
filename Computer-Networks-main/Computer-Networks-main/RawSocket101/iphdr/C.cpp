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

#define IP "127.0.0.1"
#define SRC_PORT 9000
#define DEST_PORT 8000
#define PACKET_SIZE 512

struct sockaddr_in source,dest;
void print_ip_header(char* buff)
{
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

    int sfd = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
    if(sfd < 0){
        perror("socket : ");
        exit(-1);
    }
    char buf[PACKET_SIZE];
    struct iphdr* ip_header;

    struct sockaddr_in serv;
    int len = sizeof(serv);
    int ret = recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&serv,(socklen_t*)&len);
    if (ret < 0) {
        perror("recv");
        exit(1);
    }

    // Extract the IP header
    ip_header = (struct iphdr *) buf;

    // Print the source and destination IP addresses
    printf("Source IP: %s\n", inet_ntoa(*(struct in_addr *)&ip_header->saddr));
    printf("Destination IP: %s\n", inet_ntoa(*(struct in_addr *)&ip_header->daddr));

    // Print the payload
    printf("Payload: %s\n", buf + sizeof(struct iphdr));
    print_ip_header(buf);
    
    return 0;
}