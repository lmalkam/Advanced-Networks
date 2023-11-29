#include<bits/stdc++.h>
#include <netinet/in.h>

using namespace std;
#define PORT 8000
int main(int argc, char* argv[]){
	struct sockaddr_in address;
	
	int sfd = socket(AF_INET, SOCK_STREAM,0);
	
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	
	int status = connect(sfd,(struct sockaddr*) &address, sizeof(address));
	if(status<0){
		perror("status ");
		exit(EXIT_FAILURE);
	}
	cout << "Connect Success\n";
	
	int type = atoi(argv[1]);
	send(sfd,&type,sizeof(type),0);
	char message[100];
	read(sfd,message,sizeof(message));
	cout << message;
	return 0;
}
	
	
	
