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
#define SERVER_PORT 8888 // Change this to your desired port number

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

void updateConnectionTable(const char *senderIP, const char *receiverIP, int sourcePort, int destPort)
{
    if (connectionTable.count < MAX_TABLE_SIZE)
    {
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
    printf("| %-15s | %-15s | %-5s | %-5s |\n", "Sender IP", "Receiver IP", "Src Port", "Dest Port");
    printf("|-----------------|-----------------|--------|--------|\n");
    for (int i = 0; i < connectionTable.count; i++)
    {
        printf("| %-15s | %-15s | %-5d | %-5d |\n",
               connectionTable.table[i].senderIP,
               connectionTable.table[i].receiverIP,
               connectionTable.table[i].sourcePort,
               connectionTable.table[i].destPort);
    }
}

void sendTableDataToClient(int clientSocket)
{
    // Send the connection table data to the client
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "Communication Table:\n| %-15s | %-15s | %-5s | %-5s |\n", "Sender IP", "Receiver IP", "Src Port", "Dest Port");
    for (int i = 0; i < connectionTable.count; i++)
    {
        sprintf(buffer + strlen(buffer), "| %-15s | %-15s | %-5d | %-5d |\n",
                connectionTable.table[i].senderIP,
                connectionTable.table[i].receiverIP,
                connectionTable.table[i].sourcePort,
                connectionTable.table[i].destPort);
    }
    send(clientSocket, buffer, sizeof(buffer), 0);
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
        updateConnectionTable(senderIP, receiverIP, tcpData->source, tcpData->dest);

        // Display the communication table
        displayConnectionTable();

        // Example: Send table data to a client
        int clientSocket = *((int *)args);
        sendTableDataToClient(clientSocket);
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

    // Socket programming setup
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) == -1)
    {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    // Accept client connections
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize);
    if (clientSocket == -1)
    {
        perror("Error accepting client connection");
        exit(EXIT_FAILURE);
    }

    // Loop to handle packets
    pcap_loop(handle, 0, packet_handler, (u_char *)&clientSocket);

    // Close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
