#include<bits/stdc++.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;

#define PORT 8080
struct candidate{
    char name[100];
    int roll;
    int done[3];
};

int main(){
	struct sockaddr_in address;
	
	int sfd;
	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("sfd fail");
		return 0;
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
	
	if(connect(sfd,(struct sockaddr*)&address,sizeof(address)) < 0){
		perror("connect failed");
		return 0;
	}
	
	int buflen=1024;
	char s[buflen];
	char* hello = "Hello from cleint";
	send(sfd, hello, strlen(hello)+1, 0);

	candidate c;
	
	char buff[sizeof(candidate)];
    read(sfd,buff, sizeof(buff));
	memcpy(&c,buff,sizeof(candidate));
    printf("%s\n", c.name);
	printf("%d\n", c.roll);
    for(int i = 0 ; i  < 3; ++i)cout << c.done[i];
    //close(sfd);
	
	return 0;
}
	
	
	
	
	
	
