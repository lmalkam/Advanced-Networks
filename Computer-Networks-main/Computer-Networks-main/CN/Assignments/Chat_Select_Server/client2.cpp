#include "cn_header.h"
#define PORT 3001 

using namespace std;

void setup(int &sfd){
    struct sockaddr_in address; 
	
	char buffer[1024] = {0}; 
	if ((sfd= socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		exit(0);
	} 

	address.sin_family = AF_INET; 
	address.sin_port = htons(PORT); 
	
	if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		exit(0);
	} 

	if (connect(sfd, (struct sockaddr *)&address, sizeof(address)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		exit(0);
	} 
	cout<<"Server Connected\n";
	cout<<"Listening on PORT "<<PORT<<"\n";
}

int main()
{
    int sfd=0;
    setup(sfd);
    while(1){
        char buff[1024] = {0};
        cin>>buff;
        send(sfd, buff, sizeof(buff), 0);
		char rec_buf[1024] = {0};
		read(sfd, rec_buf, sizeof(rec));
		cout<<"Message From Server: "<<rec_buf<<"\n";
    }
}