#include <stdio.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

void packet_handler(unsigned char *user_data, const struct pcap_pkthdr *pkthdr, const unsigned char *packet) {
    struct iphdr *ip_header = (struct iphdr *)(packet + 14); // Skip Ethernet header
    struct tcphdr *tcp_header = (struct tcphdr *)(packet + 14 + (ip_header->ihl * 4)); // Skip IP header

    if (ip_header->protocol == IPPROTO_TCP) {
        printf("Source IP: %s\n", inet_ntoa(*(struct in_addr *)&(ip_header->saddr)));
        printf("Destination IP: %s\n", inet_ntoa(*(struct in_addr *)&(ip_header->daddr)));
        printf("Source Port: %d\n", ntohs(tcp_header->source));
        printf("Destination Port: %d\n", ntohs(tcp_header->dest));

        // Assuming payload starts after TCP header
        const char *payload = (const char *)(packet + 14 + (ip_header->ihl * 4) + (tcp_header->doff * 4));

        printf("Payload: %s\n", payload);
        printf("-------------------------------------------------\n");
    }
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    const char *dev = "eth0"; // Change this to your network interface

    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);

    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return 2;
    }

    pcap_loop(handle, 0, packet_handler, NULL);

    pcap_close(handle);
    return 0;
}
