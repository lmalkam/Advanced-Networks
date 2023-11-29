#include <bits/stdc++.h>
using namespace std;

void hndfn(int sig, siginfo_t *siginfo, void* context){
	cout << "Kill pid : " << siginfo->si_pid << endl;
}


int main(){
	struct sigaction act;
    	act.sa_sigaction = hndfn;
    	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);

	int c = fork();
	if(!c){
		cout << "Child : " << getpid() << endl;
		kill(c,SIGUSR1);
	}else{
		cout << "Parent : " << getpid() << endl;
		
	}
	return 0;
}
