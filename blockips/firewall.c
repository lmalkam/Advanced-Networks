#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>

// Define the IP address to block
#define BLOCKED_IP "142.250.182.110"

// Function to process each captured packet
void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    // Extract source IP address from the captured packet
    char source_ip[16];  // Assuming IPv4 addresses
    snprintf(source_ip, sizeof(source_ip), "%d.%d.%d.%d", packet[26], packet[27], packet[28], packet[29]);

    printf("Packet captured. Source IP: %s\n", source_ip);

    // Check if the source IP matches the blocked IP
    if (strcmp(source_ip, BLOCKED_IP) == 0) {
        printf("Blocked packet from %s\n", BLOCKED_IP);
        return;  // Drop the packet
    }

    // Continue processing the packet or implement other firewall rules here

    printf("Allowed packet\n");
}

int main(int argc, char **argv) {
    char errbuf[PCAP_ERRBUF_SIZE]; // Buffer for pcap error messages

    // Check command-line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <interface>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the network interface for packet capture
    pcap_t *handle = pcap_open_live(argv[1], BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Could not open device %s: %s\n", argv[1], errbuf);
        exit(EXIT_FAILURE);
    }

    // Capture 10 packets (adjust as needed)
    int num_packets = 100;

    // Start capturing packets and call packet_handler for each captured packet
    pcap_loop(handle, num_packets, packet_handler, NULL);

    // Close the capture handle
    pcap_close(handle);

    return 0;
}
