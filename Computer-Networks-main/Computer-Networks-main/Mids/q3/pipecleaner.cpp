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

int main(){
	struct message_buffer msg;
	msqid = msgget(12345, IPC_CREAT | 0666);
	while(1){
		msgrcv(msqid, &msg, 10000, 1, 0);
	}
	
	return 0;
}
	













	
