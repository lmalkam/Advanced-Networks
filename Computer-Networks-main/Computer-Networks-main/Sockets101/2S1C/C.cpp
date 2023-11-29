#include<bits/stdc++.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;

#define PORT 8080

int main(){
	struct sockaddr_in address;
	
	int sfd;
	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("sfd fail");
		return 0;
	}
	
	address.sin_family = AF_INET;
    	address.sin_port = htons(PORT);
	
	if(connect(sfd,(struct sockaddr*)&address,sizeof(address)) < 0){
		perror("connect failed");
		return 0;
	}
    	close(sfd);
	return 0;
}
	
	
	
	
	
	
