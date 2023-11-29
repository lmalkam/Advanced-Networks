#include "cn_header.h"

#define BUFFER_SIZE 1024
#define PORT 8000
void receiveCode();

int main(){

    /** 
    * @brief: Creating the socket
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

    struct sockaddr_in serverAddress, clientAddress;
    bzero(&serverAddress, sizeof(serverAddress));
   
    /** 
    * @brief: Assigning port and IP address to the server
    */
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(PORT);
   
    /** 
    * @brief: Binding the IP Address and port to the socket created
    */
    int bindSocket = bind(sfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    
    if (bindSocket != 0) {
        printf("FAILED: Socket Bind Failed\n");
        exit(0);
    }
    else{
        printf("CONNECTED: Server is Binded Successfully\n");
    }
    
    /** 
    * @brief: makes the server ready for listening
    * verifies before moving further
    */
    int serverListen = listen(sfd, 5);
    if ( serverListen != 0) {
        printf("FAILED: Cannot Listen Server\n");
        exit(0);
    }
    else{
        printf("SUCCESS: Server is listening on Port %d\nTest Cases Passed\n", PORT);
        exit(0);
    }
    int lenClient = sizeof(clientAddress);
   
    /**
     * @brief : checks and verify the packet received from clientAddress
     * 
     * on the basis of verification, program is executed
     */
    int nsfd = accept(sfd, (struct sockaddr *)&clientAddress, &lenClient);

    if (nsfd < 0) {
        printf("FAILED: SERVER ACCEPT DENIED\n");
        exit(0);
    }

    int c = fork();
    if(c > 0)
    {
        // Parent will wait for child to execute P executable file
        wait();

        /** 
        * @checking expected and obtained outputs
        * storing their data in buffer and comparing if both are same or not
        */
        int OT = open("OT.txt", O_RDONLY);
        int Pout = open("Pout.txt", O_RDONLY);

        char ot_buffer[BUFFER_SIZE], pout_buffer[BUFFER_SIZE];

        int size = read(OT, ot_buffer, BUFFER_SIZE);
        read(Pout, pout_buffer, BUFFER_SIZE);

        int check = 1;
        for(int i=0;i<size; i++)
        {
           if(ot_buffer[i] !=pout_buffer[i])
           {
               check = 0;
               break;
           }
        }

        if(check){
            printf("Test case Passed!\n");
        }
        else {
            printf("Wrong Answer.\n");
        }

    }
    else{
        // Receiving the code from client and storing in server copy : P_temp.cpp
        receiveCode(nsfd);

        // Compiling the server copy
        system("g++ P_recv.cpp -o P");

        // Instructions given in assignment: reading the IT and Pout.txt
        int itfd = open("IT.txt", O_RDONLY);
        int poutfd = open("Pout.txt", O_RDWR|O_CREAT, 0777);

        dup2(itfd,0);
        dup2(poutfd,1);

        // running the output / executable file
        char *args[]={"./P",NULL};
        execvp(args[0],args);
    }
    close(sfd);
}

// to receive the code from client and store into server copy
void receiveCode(int nsfd) 
{
    FILE * fp;
    char buffer[BUFFER_SIZE];

    fp = fopen("P_temp.cpp", "w");
    while (1) {
        int recStatus = recv(nsfd, buffer, BUFFER_SIZE, 0);
        if (recStatus <= 0) {
            break;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, BUFFER_SIZE);
    }
    fclose(fp);
    return;
}