#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.137.192"  // Replace with the actual IP address of your Mac
#define PORT 12345

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);

    // Receive a message from the server
    char received_message[1024];
    recv(client_socket, received_message, sizeof(received_message), 0);
    printf("Received message from server: %s\n", received_message);

    // Send a message to the server
    const char* message_to_send = "Hello from the client!";
    send(client_socket, message_to_send, strlen(message_to_send), 0);

    // Close the socket
    close(client_socket);

    return 0;
}
