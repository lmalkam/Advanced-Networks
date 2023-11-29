#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/in_systm.h>
#include <string.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>

#define SNP_LEN 1024

struct PacketInfo
{
    char srcIP[INET_ADDRSTRLEN];
    char destIP[INET_ADDRSTRLEN];
    char message[100]; // Adjust the size based on your needs
};

struct PacketInfo packetInfoArray[100]; // Adjust the size based on your needs
int packetCount = 0;

void sendPacketInfo();

int stop_capture = 0; // Global flag to control packet capture

void my_callback(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    const struct ether_header *ehdr;
    struct ip *iphdr;
    const struct udphdr *udp;

    ehdr = (struct ether_header *)packet;
    iphdr = (struct ip *)(packet + 14);

    // Store information in the data structure
    strncpy(packetInfoArray[packetCount].srcIP, inet_ntoa(iphdr->ip_src), INET_ADDRSTRLEN);
    strncpy(packetInfoArray[packetCount].destIP, inet_ntoa(iphdr->ip_dst), INET_ADDRSTRLEN);

    if (iphdr->ip_p == IPPROTO_UDP)
    {
        const char *message = (const char *)(packet + sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct udphdr));
        strncpy(packetInfoArray[packetCount].message, message, sizeof(packetInfoArray[packetCount].message));

        // Check if the message is "Linesh"
        if (strstr(message, "Linesh") != NULL)
        {
            printf("Found the message 'Linesh'.\n");
            stop_capture = 1;
        }
    }

    // Increment packetCount
    packetCount++;

    // If the stop_capture flag is set, send the packet information and exit
    if (stop_capture)
    {
        sendPacketInfo();
        exit(0);
    }
}

void sendPacketInfo()
{
    int sockfd;
    struct sockaddr_in server_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Error creating socket");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // Adjust the port number based on your needs
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Adjust the IP address based on your needs

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error connecting to the server");
        close(sockfd);
        exit(1);
    }

    // Send the packet information to the server
    if (send(sockfd, packetInfoArray, sizeof(packetInfoArray), 0) == -1)
    {
        perror("Error sending packet information");
    }

    // Close the socket
    close(sockfd);
}

int main(int argc, char *argv[])
{
    char *dev = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    char filter[] = "ip";
    struct bpf_program fp;
    uint32_t net, mask;
    int np = 100;

    dev = pcap_lookupdev(errbuf);
    pcap_lookupnet(dev, &net, &mask, errbuf);

    printf("Device: %s\n", dev);
    printf("Number of packets: %d\n", np);
    printf("Filter expression: %s\n", filter);

    handle = pcap_open_live(dev, SNP_LEN, 1, 1000, errbuf);

    if (pcap_datalink(handle) != DLT_EN10MB)
    {
        fprintf(stderr, "%s is not an Ethernet\n", dev);
        exit(1);
    }

    pcap_compile(handle, &fp, filter, 0, net);
    pcap_setfilter(handle, &fp);
    pcap_loop(handle, np, my_callback, NULL);

    pcap_freecode(&fp);
    pcap_close(handle);

    printf("Capture Complete\n");
    return 0;
}
