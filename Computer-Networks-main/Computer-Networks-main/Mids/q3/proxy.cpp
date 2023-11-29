#include<bits/stdc++.h>
#include <netinet/in.h>
#include<sys/poll.h>
#include <sys/msg.h>
using namespace std;

struct sockaddr_in address;

int msqid; 

struct message_buffer{
    long type;
    int nsfd;
};


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

void* serverHandler(void* arg){
	int i = *(int*)arg;
	int sfd = socket(AF_INET, SOCK_STREAM,0);
	struct sockaddr_in clientaddr;
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(8080+i);
	int status = connect(sfd,(struct sockaddr*) &clientaddr, sizeof(clientaddr));
	if(status < 0){
		cout << "Connect Fail at " << i << endl;
		return NULL;
	}
	
	char req[] = "#";
	struct message_buffer msg;
	char message[100];
	while(1){
		msgrcv(msqid, &msg, 10000, i, 0);
		send(sfd,req,strlen(req)+1,0);
		read(sfd,message,sizeof(message));
		send(msg.nsfd,message,strlen(message)+1,0);
	}
}

int main(){

	msqid = msgget(12345, IPC_CREAT | 0666);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	int sfd = sfdMaker(8000);
	
	pthread_t pt[5];
	for(int i = 1; i <= 5; ++i){
		int* j = (int*)malloc(sizeof(int));
		*j = i;
		pthread_create(&pt[i-1],NULL,serverHandler,(void*)j);
	}
	
	int nsfd;
	struct sockaddr_in clientaddr;
	int addrlen = sizeof(clientaddr);
	while((nsfd = accept(sfd,(struct sockaddr*)&clientaddr,(socklen_t*)&addrlen)) > 0){
		cout << "Accepted" << endl;
		int type;
		read(nsfd,&type,sizeof(type));
		cout << "Read " << type << endl;
		struct message_buffer msg;
		msg.type = type;
		msg.nsfd = nsfd;
		msgsnd(msqid, &msg, sizeof(msg.type) + 1, 0);
	}
	
	return 0;
}
	













	
