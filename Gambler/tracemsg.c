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
#include <stdlib.h>
#include <string.h>

struct MyTCP
{
    uint16_t source;
    uint16_t dest;
    uint8_t con_req;
    uint8_t con_acc;
    uint8_t fin_ack;
    uint8_t data_flg;
    uint16_t seq;
    uint16_t rec_seq;
};

struct ConnectionInfo
{
    struct ether_addr senderMAC;
    struct ether_addr receiverMAC;
    char senderIP[INET_ADDRSTRLEN];
    char receiverIP[INET_ADDRSTRLEN];
    int sourcePort;
    int destPort;
    char message[1024]; // Assuming a maximum message length of 1024 bytes
};

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
                           int sourcePort, int destPort,
                           const char *message)
{
    if (connectionTable.count < MAX_TABLE_SIZE)
    {
        memcpy(&connectionTable.table[connectionTable.count].senderMAC, senderMAC, sizeof(struct ether_addr));
        memcpy(&connectionTable.table[connectionTable.count].receiverMAC, receiverMAC, sizeof(struct ether_addr));
        strcpy(connectionTable.table[connectionTable.count].senderIP, senderIP);
        strcpy(connectionTable.table[connectionTable.count].receiverIP, receiverIP);
        connectionTable.table[connectionTable.count].sourcePort = sourcePort;
        connectionTable.table[connectionTable.count].destPort = destPort;
        strncpy(connectionTable.table[connectionTable.count].message, message, sizeof(connectionTable.table[connectionTable.count].message));
        connectionTable.table[connectionTable.count].message[sizeof(connectionTable.table[connectionTable.count].message) - 1] = '\0'; // Ensure null-termination
        connectionTable.count++;
    }
    // You can add more logic here to handle a full table if needed
}

void displayConnectionTable()
{
    printf("Communication Table:\n");
    printf("| %-18s | %-18s | %-15s | %-15s | %-5s | %-5s | %-40s |\n",
           "Sender MAC", "Receiver MAC", "Sender IP", "Receiver IP", "Src Port", "Dest Port", "Message");
    printf("|------------------|------------------|-----------------|-----------------|--------|--------|------------------------------------------|\n");
    for (int i = 0; i < connectionTable.count; i++)
    {
        printf("| %02X:%02X:%02X:%02X:%02X:%02X | %02X:%02X:%02X:%02X:%02X:%02X | %-15s | %-15s | %-5d | %-5d | %-40s |\n",
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
               connectionTable.table[i].destPort,
               connectionTable.table[i].message);
    }
}

void extractAndHandleMessage(const u_char *packet, int tcp_payload_offset, int payload_length)
{
    char message[1024]; // Assuming a maximum message length of 1024 bytes

    if (payload_length > sizeof(message))
    {
        // Message too large, truncate
        payload_length = sizeof(message);
    }

    strncpy(message, (const char *)(packet + tcp_payload_offset), payload_length);
    message[payload_length - 1] = '\0'; // Ensure null-termination

    // Add logic to handle the extracted message
    printf("Extracted Message: %s\n", message);
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

        // Extract message from TCP payload
        int tcp_payload_offset = 14 + (ip->ihl * 4) + (tcpData->th_off * 4);
        int payload_length = ntohs(ip->tot_len) - (ip->ihl * 4) - (tcpData->doff * 4);

        // Add logic to handle communication information
        updateConnectionTable((struct ether_addr *)ether->ether_shost,
                              (struct ether_addr *)ether->ether_dhost,
                              senderIP, receiverIP,
                              tcpData->source, tcpData->dest,
                              "");

        // Display the communication table
        displayConnectionTable();

        // Extract and handle the message
        extractAndHandleMessage(packet, tcp_payload_offset, payload_length);
    }
}

int main(int argc, char const *argv[])
{
    char error_buffer[PCAP_ERRBUF_SIZE];

    handle = pcap_open_live(device, BUFSIZ, 1, 1000, error_buffer);
    if (handle == NULL)
    {
        fprintf(stderr, "Could not open device %s: %s\n", device, error_buffer);
        return 2;
    }

    connectionTable.count = 0;

    if (pcap_loop(handle, 0, packet_handler, NULL) < 0)
    {
        fprintf(stderr, "pcap_loop failed: %s\n", pcap_geterr(handle));
        return 2;
    }

    pcap_close(handle);

    return 0;
}
