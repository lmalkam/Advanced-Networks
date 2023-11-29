#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
using namespace std;

#define s_type 3
#define r_type 4
#define mq_id 12345
#define MAX_LENGTH 100000

int left_neigh = -1, right_neigh = -1;

struct message_buffer {
    long type;
    int pid;
};

void hndfn(int sig, siginfo_t *siginfo, void* context){
	cout << "ouch "<< siginfo->si_pid << endl;
	if(left_neigh == -1)
		left_neigh = siginfo->si_pid;
	else
		cout << "SIGUSR1 Received\n";
}

void hndfn1(int sig, siginfo_t *siginfo, void* context){
	cout << "SIGUSR2 Received\n";
}

int main(){
	struct sigaction act;
    	act.sa_sigaction = hndfn;
    	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	struct sigaction act1;
    	act1.sa_sigaction = hndfn1;
    	act1.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR2, &act1, NULL);
	
	cout << getpid() << endl;
	int msqid;
	struct message_buffer msg;
	
	msqid = msgget(mq_id, IPC_CREAT | 0666);
	
	
	msg.pid = getpid();
	msg.type = s_type;

	msgsnd(msqid, &msg, sizeof(msg.pid) + 1, 0);
	
	msgrcv(msqid, &msg, MAX_LENGTH, r_type, 0);
	right_neigh = msg.pid;
	while(right_neigh == getpid()){
		msgrcv(msqid, &msg, MAX_LENGTH, r_type, 0);
		right_neigh = msg.pid;
	}
	kill(right_neigh, SIGUSR1);

	while(left_neigh == -1){}
	
	cout << "p3 : " << getpid() << endl << left_neigh << " " << right_neigh << endl;

	for(int i = 0; i < 3; ++i){
		pause();
		kill(right_neigh, SIGUSR1);
	}
	
	for(int i = 0; i < 3; ++i){
		pause();
		kill(left_neigh, SIGUSR2);
	}

	return 0;
	
}
