#include <bits/stdc++.h>
#include <sys/ipc.h>
using namespace std;

void hndfn(int sig, siginfo_t* singing, void* context){
	cout << "AH GOT ME\n";
	exit(EXIT_FAILURE);
}

int main(){
	struct sigaction act;
	act.sa_sigaction = hndfn;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	
	cout << "Hey $";
	string s;
	cin >> s;
	cout << s;		
	return 0;
}
