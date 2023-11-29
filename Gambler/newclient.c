#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1" // Change this to the server's IP address
#define SERVER_PORT 8888      // Change this to the server's port number

void receiveTableData(int socket)
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Receive data from the server
    recv(socket, buffer, sizeof(buffer), 0);

    // Display received data
    printf("Received Table Data:\n%s", buffer);
}

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &(serverAddr.sin_addr));

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Error connecting to the server");
        exit(EXIT_FAILURE);
    }

    // Receive communication table data
    receiveTableData(clientSocket);

    // Close the socket
    close(clientSocket);

    return 0;
}
