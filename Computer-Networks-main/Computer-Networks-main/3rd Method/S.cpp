#include<bits/stdc++.h>
#include<netinet/in.h>
#include<sys/poll.h>
using namespace std;

int N = 3;
int port = 8000;
pair<int,struct sockaddr_in> sfdGiver(int i){
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port+i); 
	int opt = 1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,sizeof(opt)); 
	bind(sfd,(struct sockaddr*)&addr,sizeof(addr));
	listen(sfd,2);
	return {sfd,addr};
}

int main(){
	pair<int,struct sockaddr_in> sfds[N];
	for(int i = 0; i < N; ++i)
		sfds[i] = sfdGiver(i);
	
	char pnames[N][10] = { "./A", "./B", "./C" };	
	
	struct pollfd pfds[N];
	
	for(int i = 0; i < N; ++i){
		pfds[i].fd = sfds[i].first;
		pfds[i].events |= POLLIN;
		pfds[i].revents = 0;
	}
	
	
	while(1){
		int ne = poll(pfds,N,4*1000);
		cout << ne << endl;
		if(ne < 1)continue;
		for(int i = 0; i < N; ++i){
			if(pfds[i].revents & POLLIN){
				int pp[2];
				pipe(pp);
				int c = fork();
				cout << "yep\n";
				int addrlen = sizeof(struct sockaddr_in);
				int nsfd = accept(pfds[i].fd,(struct sockaddr*) &sfds[i].second, (socklen_t*)&addrlen);
				if(!c){
					close(pp[1]);
					dup2(pfds[i].fd,20);
					dup2(nsfd,21);
					dup2(pp[0],22);
					execv(pnames[i],NULL);	
				}else{
					close(pp[0]);
					close(nsfd);
					write(pp[1],&sfds[i].second,sizeof(sfds[i].second));
					close(pfds[i].fd);
				}
				break;
			}
		}
	}
	
	return 0;
}
				
				
	
	
	
	
