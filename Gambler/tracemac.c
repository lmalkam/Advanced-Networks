#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <sys/un.h>
#include <pcap.h>
#include <pthread.h>
#include "Protocol.h"
#include <stdlib.h>
#include <string.h>

#define KEYPATH "."
#define MAX_TABLE_SIZE 100

struct RPC_args
{
    int op1, op2; //,op;
};

struct RPC_hdr
{
    char func_name[10];
    int req;
    int result;
    struct RPC_args arg;
};

struct ConnectionInfo
{
    struct ether_addr senderMAC;
    struct ether_addr receiverMAC;
    char senderIP[INET_ADDRSTRLEN];
    char receiverIP[INET_ADDRSTRLEN];
    int sourcePort;
    int destPort;
};

// Hash table to store communication information
struct ConnectionInfoTable
{
    struct ConnectionInfo table[MAX_TABLE_SIZE];
    int count;
};

char device[10] = "lo";
pcap_t *handle;
struct ConnectionInfoTable connectionTable;

void updateConnectionTable(const struct ether_addr *senderMAC, const struct ether_addr *receiverMAC,
                           const char *senderIP, const char *receiverIP,
                           int sourcePort, int destPort)
{
    if (connectionTable.count < MAX_TABLE_SIZE)
    {
        memcpy(&connectionTable.table[connectionTable.count].senderMAC, senderMAC, sizeof(struct ether_addr));
        memcpy(&connectionTable.table[connectionTable.count].receiverMAC, receiverMAC, sizeof(struct ether_addr));
        strcpy(connectionTable.table[connectionTable.count].senderIP, senderIP);
        strcpy(connectionTable.table[connectionTable.count].receiverIP, receiverIP);
        connectionTable.table[connectionTable.count].sourcePort = sourcePort;
        connectionTable.table[connectionTable.count].destPort = destPort;
        connectionTable.count++;
    }
    // You can add more logic here to handle a full table if needed
}

void displayConnectionTable()
{
    printf("Communication Table:\n");
    printf("| %-18s | %-18s | %-15s | %-15s | %-5s | %-5s |\n",
           "Sender MAC", "Receiver MAC", "Sender IP", "Receiver IP", "Src Port", "Dest Port");
    printf("|------------------|------------------|-----------------|-----------------|--------|--------|\n");
    for (int i = 0; i < connectionTable.count; i++)
    {
        printf("| %02X:%02X:%02X:%02X:%02X:%02X | %02X:%02X:%02X:%02X:%02X:%02X | %-15s | %-15s | %-5d | %-5d |\n",
               connectionTable.table[i].senderMAC.ether_addr_octet[0],
               connectionTable.table[i].senderMAC.ether_addr_octet[1],
               connectionTable.table[i].senderMAC.ether_addr_octet[2],
               connectionTable.table[i].senderMAC.ether_addr_octet[3],
               connectionTable.table[i].senderMAC.ether_addr_octet[4],
               connectionTable.table[i].senderMAC.ether_addr_octet[5],
               connectionTable.table[i].receiverMAC.ether_addr_octet[0],
               connectionTable.table[i].receiverMAC.ether_addr_octet[1],
               connectionTable.table[i].receiverMAC.ether_addr_octet[2],
               connectionTable.table[i].receiverMAC.ether_addr_octet[3],
               connectionTable.table[i].receiverMAC.ether_addr_octet[4],
               connectionTable.table[i].receiverMAC.ether_addr_octet[5],
               connectionTable.table[i].senderIP,
               connectionTable.table[i].receiverIP,
               connectionTable.table[i].sourcePort,
               connectionTable.table[i].destPort);
    }
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
        if (ip->protocol != 253)
            return;

        char senderIP[INET_ADDRSTRLEN];
        char receiverIP[INET_ADDRSTRLEN];
        struct MyTCP *tcpData;
        tcpData = (struct MyTCP *)(packet + 14 + (ip->ihl * 4));

        inet_ntop(AF_INET, &(ip->saddr), senderIP, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ip->daddr), receiverIP, INET_ADDRSTRLEN);

        // Add logic to handle communication information
        updateConnectionTable((struct ether_addr *)ether->ether_shost,
                              (struct ether_addr *)ether->ether_dhost,
                              senderIP, receiverIP,
                              tcpData->source, tcpData->dest);

        // Display the communication table
        displayConnectionTable();
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

    // Initialize the connection table
    connectionTable.count = 0;

    pcap_loop(handle, 0, packet_handler, NULL);

    return 0;
}
