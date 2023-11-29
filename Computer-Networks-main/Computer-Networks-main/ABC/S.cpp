#include<bits/stdc++.h>
#include<sys/poll.h>
#include<netinet/in.h>
using namespace std;
#define PORT 8080
int n = 3;
int ptr = 0;


void hndfn(int s, siginfo_t *info, void* context){
	ptr++;
	return;
}

int main(){
	struct sigaction act;
	act.sa_sigaction = hndfn;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1,&act,NULL);
	char* names[] = {"./A", "./B", "./D"};
	
	
	while(1){
		if(ptr == n){
			cout << "Continue ? : ";
			char c; cin >> c;
			if(c!='y')break;
			ptr = 0;
		}
		int pid;
		char lol[100] = "pidof ";
		FILE* fp = popen(strcat(lol,names[ptr]),"r");
		fscanf(fp,"%d",&pid);
		cout << pid << endl;
		kill(pid,SIGUSR1);
		pause();
	}
	cout << "Server Quits\n";
	return 0;
}	
	
	
	
	
