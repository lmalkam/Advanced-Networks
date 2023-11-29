#include<bits/stdc++.h>
#include<netinet/in.h>
#define PORT 8080
using namespace std;

struct p{
	int nsfd;
	int i;
};

void* service(void* arg){
	struct p pp = *(p*)arg;
	char s[100];
	sprintf(s,"Hello from Child : %d",pp.i);
	while(1){
		send(pp.nsfd,s,strlen(s)+1,0);
		sleep(5);
	}
}

void serve(int ii, struct sockaddr_in &addr){
	pthread_t pt[3];
	for(int i = 0; i < 3; ++i){
		int addrlen = sizeof(addr);
		int nsfd = accept(20,(struct sockaddr*) &addr, (socklen_t*) &addrlen);
		cout << ii << " " << i << " accepted "<< endl;
		struct p pp;
		pp.nsfd = nsfd;
		pp.i = ii;
		pthread_create(&pt[i],NULL,service,(void*)&pp);
	}
	kill(getppid(),SIGUSR1);
	for(int i = 0; i < 3; ++i)
		pthread_join(pt[i],NULL);
	
	close(20);
}

void hndfn(int s, siginfo_t* info, void* context){
	cout << "sigusr1\n";
	return;
}

int main(){
	struct sigaction act;
	act.sa_sigaction = hndfn;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1,&act,NULL);

	int n = 3;
	struct sockaddr_in addr;
	
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	
	int opt = 1;
	setsockopt(sfd,SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	bind(sfd,(struct sockaddr*) &addr, sizeof(addr));
	listen(sfd,4);
	dup2(sfd,20);
	int sf = getpid();
	for(int i = 0; i < n; ++i){
		int c = fork();
		if(!c){
			serve(i,addr);
		}
	}
	for(int i = 0; i < n; ++i)
		pause();
	serve(69,addr);
	return 0;
}
	
	
	
