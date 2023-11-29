#include<bits/stdc++.h>
#include<netinet/in.h>
using namespace std;

#define PORT 8080

int main(){
	struct sockaddr_in address;
	
	int sfd;
	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("sfd fail");
		return 0;
	}
	int opt = 1;
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        	perror("setsockopt");
        	return 0;
    	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	if(bind(sfd,(struct sockaddr*) &address, sizeof(address)) < 0){
		perror("bind failed");
		return 0;
	}
	
	if(listen(sfd,3) < 0){
		perror("listen failed");
		return 0;
	}
	cout << address.sin_addr.s_addr << endl;
	int nsfd;
	int addrlen = sizeof(address);
	if((nsfd = accept(sfd,(struct sockaddr*) &address, (socklen_t*) &addrlen)) < 0){
		perror("accept failed");
		return 0;
	}
	cout << "Yay \n";
	
	close(nsfd);
	shutdown(sfd,SHUT_RDWR);
	
	return 0;
}
	
	
	
	
	
	
