#include<bits/stdc++.h>
#include<netinet/in.h>
#include <sys/un.h>
#include<sys/socket.h>

using namespace std;

#define portC 8000

int main(){
    struct sockaddr_in address;
	
	int sfd;
	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("sfd fail");
		return 0;
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(portC);
	
	if(connect(sfd,(struct sockaddr*)&address,sizeof(address)) < 0){
		perror("connect failed");
		return 0;
	}
    cout << "Service Alloted\n";

    char tosend[1024];
    char message[1024];
    while(cin.getline(tosend,sizeof(tosend))){
        send(sfd,tosend,strlen(tosend)+1,0);
        read(sfd,message,sizeof(message));
        cout << "Received : " << message << endl;
    }
    return 0;
}