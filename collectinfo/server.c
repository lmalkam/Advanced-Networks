#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_PACKETS 100

struct PacketInfo
{
    char srcIP[INET_ADDRSTRLEN];
    char destIP[INET_ADDRSTRLEN];
    char message[100];
};

void receivePacketInfo();

int main()
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Error creating socket");
        exit(1);
    }

    // Configure server address
    portno = 8080; // Adjust the port number based on your needs
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("Error binding socket");
        close(sockfd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) == -1)
    {
        perror("Error listening for connections");
        close(sockfd);
        exit(1);
    }

    printf("Server listening on port %d...\n", portno);

    // Accept incoming connections
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd == -1)
    {
        perror("Error accepting connection");
        close(sockfd);
        exit(1);
    }

    printf("Client connected.\n");

    // Receive packet information from the client
    receivePacketInfo(newsockfd);

    // Close sockets
    close(newsockfd);
    close(sockfd);

    return 0;
}

void receivePacketInfo(int sockfd)
{
    struct PacketInfo packetInfoArray[MAX_PACKETS];

    // Receive packet information from the client
    if (recv(sockfd, packetInfoArray, sizeof(packetInfoArray), 0) == -1)
    {
        perror("Error receiving packet information");
    }
    else
    {
        // Process the received packet information
        for (int i = 0; i < MAX_PACKETS; i++)
        {
            if (strlen(packetInfoArray[i].srcIP) > 0)
            {
                if(strcmp(packetInfoArray[i].srcIP,"172.1.1.2") == 0)
                {
                printf("Packet %d:\n", i + 1);
                printf("  Source IP: %s\n", packetInfoArray[i].srcIP);
                printf("  Destination IP: %s\n", packetInfoArray[i].destIP);
                printf("  Message: %s\n", packetInfoArray[i].message);
                }
            }
        }
    }
}
