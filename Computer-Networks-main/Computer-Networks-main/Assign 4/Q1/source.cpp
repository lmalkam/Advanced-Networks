#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
using namespace std;

#define s_type 2
#define r_type 3

#define mq_id 12345
#define MAX_LENGTH 100000

int left_neigh = -1, right_neigh = -1;

struct message_buffer {
    long type;
    int pid;
};

int main(){

	int msqid;
	struct message_buffer msg;
	
	msqid = msgget(mq_id, IPC_CREAT | 0666);
	
	int c = fork();
	if(!c)
		while(msgrcv(msqid, &msg, MAX_LENGTH, 1, 0)){cout << msg.pid << endl;}
	else{
		int cc = fork();
		if(!cc)
			while(msgrcv(msqid, &msg, MAX_LENGTH, 2, 0)){cout << msg.pid << endl;}
		else{
			int ccc = fork();
			if(!ccc)
			while(msgrcv(msqid, &msg, MAX_LENGTH, 3, 0)){cout << msg.pid << endl;}
			else
			while(msgrcv(msqid, &msg, MAX_LENGTH, 4, 0)){cout << msg.pid << endl;}
		}
	}
	cout << "clean";

	return 0;
	
}
