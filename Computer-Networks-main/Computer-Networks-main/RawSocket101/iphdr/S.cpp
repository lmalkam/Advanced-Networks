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
#define SRC_PORT 8000
#define DEST_PORT 9000
#define PACKET_SIZE 512


unsigned short in_cksum(unsigned short *addr, int len) {
    int nleft = len;
    int sum = 0;
    unsigned short *w = addr;
    unsigned short answer = 0;

    // Add 16-bit words
    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }

    // Add any remaining bytes
    if (nleft == 1) {
        *(unsigned char *) (&answer) = *(unsigned char *) w;
        sum += answer;
    }

    // Add carry bits
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    // Take one's complement of the sum
    answer = ~sum;

    return answer;
}


int main(){

    int sfd = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
    if(sfd < 0){
        perror("socket : ");
        exit(-1);
    }
    char message[] = "Yo yolo wassssssssssssssup";
    char buf[PACKET_SIZE];

    struct sockaddr_in clien;
    clien.sin_family = AF_INET;
    clien.sin_addr.s_addr = inet_addr(IP);

    struct iphdr *ip_header;

    ip_header = (struct iphdr*)buf;

    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = sizeof(struct iphdr) + strlen(message);
    ip_header->id = htons(12345);
    ip_header->frag_off = 0;
    ip_header->ttl = 255;
    ip_header->protocol = IPPROTO_TCP;
    ip_header->check = 0;
    ip_header->saddr = inet_addr(IP);
    ip_header->daddr = inet_addr(IP);

    // memcpy(buf+sizeof(struct iphdr),message,strlen(message)+1);
    strcpy(buf+(32*5),message);

    ip_header->check = htons(in_cksum((unsigned short*)buf,sizeof(iphdr)));
    cout << ip_header->check;

    // int res = send(sfd,buf,ip_header->tot_len,0);
    int res = sendto(sfd,buf,ip_header->tot_len,0,(struct sockaddr*)&clien,sizeof(clien));
    if(res < 0){
        perror("send : ");
        exit(-1);
    }

    return 0;
}