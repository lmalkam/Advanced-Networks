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
#define PORT 12346
#define BUFFER_SIZE 2048

void process_udp_packet(const struct udphdr *udp_header, const char *payload, int payload_size);

int main() {
    int sockfd, recv_len;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create socket
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Prepare server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // Bind socket to server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Receive loop
    while (1) {
        recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len);

        if (recv_len < 0) {
            perror("Receive failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // Process received UDP packet
        process_udp_packet((struct udphdr *)(buffer + sizeof(struct iphdr)), buffer + sizeof(struct iphdr) + sizeof(struct udphdr), recv_len - sizeof(struct iphdr) - sizeof(struct udphdr));
    }

    // Close the socket (unreachable in this example)
    close(sockfd);

    return 0;
}

void process_udp_packet(const struct udphdr *udp_header, const char *payload, int payload_size) {
    printf("Received UDP packet:\n");
    printf("Source Port: %u\n", ntohs(udp_header->source));
    printf("Destination Port: %u\n", ntohs(udp_header->dest));
    printf("UDP Length: %u\n", ntohs(udp_header->len));

    // Print payload as a string (assuming it's a null-terminated string)
    printf("Payload: %.*s\n", payload_size, payload);
    printf("--------------------\n");
    const char *message = (const char *)(payload+ sizeof(struct ether_header) + sizeof(struct ip) +
                                             sizeof(struct udphdr));

        printf("Message:%s\n", message);
}
