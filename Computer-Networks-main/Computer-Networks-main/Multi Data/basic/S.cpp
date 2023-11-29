#include<bits/stdc++.h>
#include <netinet/in.h>
#include<sys/wait.h>
using namespace std;

#define PORT 8000
int main(){
	struct sockaddr_in address;
	
	int sfd = socket(AF_INET, SOCK_STREAM,0);
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	int opt = 1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));

	bind(sfd,(struct sockaddr*)&address,sizeof(address));
	
	listen(sfd,3);
	int addrlen = sizeof(address);
	int nsfd = accept(sfd,(struct sockaddr*)&address,(socklen_t*)&addrlen);
	
	int c = fork();
	
	if(!c){
		close(sfd);
		dup2(nsfd,20);
		execv("./D",NULL);
	}
	wait(NULL);
	close(nsfd);
	return 0;
}
	
	
	
