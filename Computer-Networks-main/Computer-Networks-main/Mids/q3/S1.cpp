#include<bits/stdc++.h>
#include <netinet/in.h>
#include<sys/poll.h>
#include <sys/msg.h>
using namespace std;

struct sockaddr_in address;

int sfdMaker(int port){
	int sfd = socket(AF_INET, SOCK_STREAM,0);
	address.sin_port = htons(port);
	int opt = 1;
	int addrlen = sizeof(address);
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));
	bind(sfd,(struct sockaddr*)&address,sizeof(address));
	listen(sfd,2);
	return sfd;
}

int main(){
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	int sfd = sfdMaker(8081);
	int nsfd;
	struct sockaddr_in clientaddr;
	int addrlen = sizeof(clientaddr);
	nsfd = accept(sfd,(struct sockaddr*)&clientaddr,(socklen_t*)&addrlen);
	
	char response[] = "Service from S1\n";
	char req[10];
	
	while(read(nsfd,req,sizeof(req))){
		send(nsfd,response,strlen(response)+1,0);
	}
	return 0;
}
	













	
