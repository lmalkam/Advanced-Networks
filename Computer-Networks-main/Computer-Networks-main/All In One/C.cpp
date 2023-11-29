#include<bits/stdc++.h>
#include<netinet/in.h>
#include <sys/un.h>
#include<sys/socket.h>
#include<sys/poll.h>
using namespace std;


int main(int argc, char* argv[]){
    int port = atoi(argv[1]) + 8000;

    sockaddr_in serv;
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(port);
	
	if(connect(sfd,(struct sockaddr*)&serv,sizeof(serv)) < 0){
		perror("connect failed");
		return 0;
	}

    char response[1000];
    read(sfd,response,sizeof(response));
    cout << response << endl;

}