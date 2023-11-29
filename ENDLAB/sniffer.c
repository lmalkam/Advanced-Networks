#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/select.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <sys/un.h>
#include <pcap.h>
#include <pthread.h>

struct Linesh_TCP
{
    uint16_t source;
    uint16_t dest;
    uint8_t connect_req;
    uint8_t connect_acc;
    uint8_t final_acknow;
    uint8_t data_flg;
    uint16_t seq;
    uint16_t rec_seq;
};

char device[10] = "lo";

pcap_t *handle;

void printip(struct iphdr *ip)
{
    printf("Printing IP header of the packet **********\n");
    printf("IP Version: %d\n", ip->version);
    printf("IP Length: %d\n", ip->ihl);
    printf("IP Protocol: %d\n", ip->protocol);
    char ad[INET_ADDRSTRLEN];
    printf("Sender IP: %s\n", inet_ntop(AF_INET, &(ip->saddr), ad, INET_ADDRSTRLEN));
    printf("Receiver IP: %s\n", inet_ntop(AF_INET, &(ip->daddr), ad, INET_ADDRSTRLEN));
}

void printtcp(struct Linesh_TCP *tcp)
{
    printf("Printing TCP header of the packet ********\n");
    printf("Source PORT: %d\n", tcp->source);
    printf("Destination PORT: %d\n", tcp->dest);
    printf("Connection Request Flag: %d\n", tcp->connect_req);
    printf("Connection Accepted Flag: %d\n", tcp->connect_acc);
    printf("Final Acknowledgement flag : %d\n", tcp->final_acknow);
    printf("Sequence NO: %d\n", tcp->rec_seq);
    printf("Data Flag: %d\n", tcp->data_flg);
}

void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    struct ether_header *ether;
    struct iphdr *ip;
    struct tcphdr *tcp;
    ether = (struct ether_header *)packet;
    if (ntohs(ether->ether_type) == ETHERTYPE_IP)
    {
        ip = (struct iphdr *)(packet + 14);
         if(ip->protocol!=253)
         	return;
        printip(ip);
        struct Linesh_TCP *tcp;
        tcp = (struct Linesh_TCP *)(packet + 14 + (ip->ihl * 4));
        printtcp(tcp);
        printf("\n\n\n");
    }
}

int main(int argc, char const *argv[])
{
    char error[PCAP_ERRBUF_SIZE];
    handle = pcap_open_live(device, BUFSIZ, 0, 1000, error);
    if (handle == NULL)
    {
        printf("Could not find handle: %s\n", error);
        exit(0);
    }

    pcap_loop(handle, 0, packet_handler, NULL);
    return 0;
}