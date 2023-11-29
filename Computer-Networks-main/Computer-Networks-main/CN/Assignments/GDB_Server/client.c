#include "cn_header.h"

#define BUFFER_SIZE 1024
#define PORT 8000

void sendCode(int);

int main(){

    /** 
    * @brief: Creating the socket and verifying
    */
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) 
    {
        printf("FAILED [401]: Socket Cannot be created\n");
        exit(0);
    }
    else{
        printf("SUCCESS [200]: Socket Created Successfully\n");
    }
        

    struct sockaddr_in serverAddress;

    bzero(&serverAddress, sizeof(serverAddress));
    
    /** 
    * @brief: Binding the IP Address and port to the socket created
    */
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(PORT);
    
    /** 
    * @brief: Connecting the server
    * and verifying the server connection
    */
    int serverConnect = connect(sfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    if (serverConnect != 0) {
        printf("CONNECTED: Server is Connected on port %d\n",PORT);
        exit(0);
    }
    else{
        printf("CONNECTED: Server is Connected on port %d\n",PORT);
    }
        
   /** 
    * @brief: Sending the code to GDB Server
    */
    sendCode(sfd);
   
    close(sfd);
}

void sendCode(int sfd) {
    FILE *fp = fopen("P.cpp", "r");
    char buffer[BUFFER_SIZE];

    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) 
    {
        if (send(sfd, buffer, sizeof(buffer), 0) == -1) 
        {
            perror("Error: Sending File");
            exit(1);
        }
        bzero(buffer, BUFFER_SIZE);
    }
}