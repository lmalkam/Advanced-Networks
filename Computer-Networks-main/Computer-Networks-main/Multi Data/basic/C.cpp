#include<bits/stdc++.h>
#include <netinet/in.h>

using namespace std;

#define PORT 8000
int main(){
	struct sockaddr_in address;
	int sfd = socket(AF_INET, SOCK_STREAM,0);
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	int status = connect(sfd,(struct sockaddr*) &address, sizeof(address));
	if(status < 0){
		perror("connect ");
		exit(0);
	}
	cout << "Connect Success\n";
	
	char s[100];
	cin.getline(s,100);
	
	send(sfd,s,strlen(s)+1,0);
	
	read(sfd,s,100);
	cout << s;
	
	return 0;
}
	
	
	
