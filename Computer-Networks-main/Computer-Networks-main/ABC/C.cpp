#include<bits/stdc++.h>
#include <netinet/in.h>

using namespace std;
#define PORT 8080
int main(int argc, char* argv[]){
	struct sockaddr_in address;
	
	int sfd = socket(AF_INET, SOCK_STREAM,0);
	cout << getpid() << endl;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	int status = connect(sfd,(struct sockaddr*) &address, sizeof(address));
	if(status<0){
		perror("status ");
		exit(EXIT_FAILURE);
	}
	cout << "Connect Success\n";	
	char s[100];
	while(read(sfd,s,100))
		cout << s;
	
	return 0;
}
	
	
	
