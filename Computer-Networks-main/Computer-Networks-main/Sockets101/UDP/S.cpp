#include<bits/stdc++.h>
#include<netinet/in.h>
#include <arpa/inet.h>
using namespace std;



int main(){
    struct sockaddr_in serv, clien;

    int sfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sfd<0){perror("sfd fail ");return 0;}

    serv.sin_family = AF_INET;
    serv.sin_port = htons(8000);
    serv.sin_addr.s_addr = INADDR_ANY;


    int opt = 1;
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        	perror("setsockopt");
        	return 0;
    }

    if(bind(sfd,(struct sockaddr *) &serv,sizeof(serv))==-1)perror("\n bind : ");
	else printf("\n bind successful ");


    char buff[1024];

    socklen_t clienlen = sizeof(clien);
    int n = recvfrom(sfd,buff,1024,MSG_WAITALL,(sockaddr*)&clien,&clienlen);
    cout << buff << endl;

    strcpy(buff,"Hello from server");

    sendto(sfd,buff,strlen(buff)+1,MSG_CONFIRM,(sockaddr*)&clien,clienlen);


    cout << "Server\n";
    socklen_t servlen = sizeof(serv); 
    int result = getsockname(sfd, (struct sockaddr *)&serv, &servlen);
    if (result == -1) {
        std::cerr << "Error getting socket name\n";
        return 1;
    }
    cout << "IP address: " << inet_ntoa(serv.sin_addr) << std::endl;
    cout << "Port number: " << ntohs(serv.sin_port) << std::endl;

    cout << "Client\n";
     result = getsockname(sfd, (struct sockaddr *)&clien, &clienlen);
    if (result == -1) {
        std::cerr << "Error getting socket name\n";
        return 1;
    }
    cout << "IP address: " << inet_ntoa(clien.sin_addr) << std::endl;
    cout << "Port number: " << ntohs(clien.sin_port) << std::endl;
    return 0;
}