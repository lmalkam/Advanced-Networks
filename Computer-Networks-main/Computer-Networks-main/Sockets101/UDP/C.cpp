#include <bits/stdc++.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

int main()
{
    struct sockaddr_in serv;

    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd < 0)
    {
        perror("sfd fail ");
        return 0;
    }

    serv.sin_family = AF_INET;
    serv.sin_port = htons(8000);

    char buff[1024];

    socklen_t servlen = sizeof(serv);
    strcpy(buff, "Hello from cleint");
    sendto(sfd, buff, strlen(buff) + 1, MSG_CONFIRM, (sockaddr *)&serv, servlen);

    int n = recvfrom(sfd, buff, 1024, MSG_WAITALL, (sockaddr *)&serv, &servlen);
    cout << buff << endl;


    cout << "Server\n";
    // socklen_t servlen = sizeof(serv); 
    int result = getsockname(sfd, (struct sockaddr *)&serv, &servlen);
    if (result == -1) {
        std::cerr << "Error getting socket name\n";
        return 1;
    }
    cout << "IP address: " << inet_ntoa(serv.sin_addr) << std::endl;
    cout << "Port number: " << ntohs(serv.sin_port) << std::endl;

    return 0;
}