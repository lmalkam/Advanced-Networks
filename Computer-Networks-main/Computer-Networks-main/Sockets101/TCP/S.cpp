#include<bits/stdc++.h>
#include<netinet/in.h>
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
	int addrlen;
	while(1){
	if((nsfd = accept(sfd,(struct sockaddr*) &address, (socklen_t*) &addrlen)) < 0){
		perror("accept failed");
		return 0;
	}
	

	int buflen=1024;
	char s[buflen];
	int valread = read(nsfd,s,buflen);
	cout << valread << endl;
	printf("REad : %s\n", s);
	
	// char* hello = "Hello from server";
	// send(nsfd,hello,strlen(hello)+1,0);
	
	candidate c;
	strcpy(c.name,"Prahtm");
	for(int i = 0 ; i  < 3; ++i)c.done[i] = false;
	c.roll = 123;

	char buff[sizeof(candidate)];
	memcpy(buff,&c,sizeof(candidate));
	send(nsfd,buff,sizeof(buff),0);

	// close(nsfd);
	// shutdown(sfd,SHUT_RDWR);
	// sleep(40);
	}
	return 0;
}
	
	
	
	
	
	
