#include<bits/stdc++.h>
#include <netinet/in.h>
#include<sys/poll.h>
using namespace std;


int main(){
	int sfd = 20;
	
	int nsfd;
	struct sockaddr_in clientaddr;
	int addrlen = sizeof(clientaddr);
	
	while((nsfd = accept(sfd,(struct sockaddr*)&clientaddr,(socklen_t*)&addrlen)) > 0){
		char message[] = "Hello From S3\n";
		send(nsfd,message,strlen(message)+1,0);
	}
	return 0;
}
	
	
