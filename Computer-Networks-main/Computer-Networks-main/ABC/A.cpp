#include<bits/stdc++.h>
#include<sys/poll.h>
#include<netinet/in.h>
using namespace std;
#define PORT 8080

void hndfn(int s, siginfo_t *info, void* context){
	cout << "Bruh\n";
	return;
}


void* service(void* arg){
	int nsfd = *(int*)arg;
	char s[] = "From A :)\n";
	while(1){
		send(nsfd,s,strlen(s)+1,0);
		sleep(4);
	}
}


int main(){
cout << getpid() << endl;
	struct sigaction act;
	act.sa_sigaction = hndfn;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1,&act,NULL);
	
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	
	
	bool doneonce = false;
	int nsfds[2];
	pthread_t pt[2];
	while(1){
		pause();
		struct sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_port = htons(PORT);
		int sfd = socket(AF_INET, SOCK_STREAM,0);
		if(sfd<0)perror("sfd : ");
		address.sin_addr.s_addr = INADDR_ANY;
		int opt = 1;
		int addrlen = sizeof(address);
		int ssop = setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));
		if(ssop)perror("ssop : ");
		int bindi = bind(sfd,(struct sockaddr*)&address,sizeof(address));
		if(bindi)perror("bindi : ");
		int lisni = listen(sfd,2);
		if(lisni)perror("lisni : ");
		cout << "Listening : ";
		cout.flush();
		if(doneonce){
			for(int i = 0; i < 2; ++i){
				close(nsfds[i]);
			}
		}
		doneonce = true;
		for(int i = 0; i < 2; ++i){
			nsfds[i] = accept(sfd,(struct sockaddr*)&address,(socklen_t*)&addrlen);
			char s[] = "Hey\n";
			send(nsfds[i],s,strlen(s)+1,0);
			pthread_create(&pt[i],NULL,service,(void*)&nsfds[i]);
			//pthread_join(pt[i],NULL);
		}
		int pid;
		FILE* fp = popen("pidof ./S", "r");
		fscanf(fp,"%d",&pid);
		kill(pid,SIGUSR1);
		close(sfd);
	}
	return 0;
}	
	
	
	
	
