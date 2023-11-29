#include<bits/stdc++.h>
#include <netinet/in.h>
#include<sys/poll.h>
using namespace std;

#define PORT 8000

/*int fullconnect(int port, sockaddr_int &address){
	int opt = 1;
	int sfd1 = socket(AF_INET, SOCK_STREAM,0);
	address.sin_port = htons(port);
	setsockopt(sfd1,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));
	bind(sfd1,(struct sockaddr*)&address,sizeof(address));
	listen(sfd1,10);
	return sfd1;
}*/
	
int main(){
	struct sockaddr_in address;
	int sfd = socket(AF_INET, SOCK_STREAM,0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	int opt = 1;
	int addrlen = sizeof(address);
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));
	bind(sfd,(struct sockaddr*)&address,sizeof(address));
	listen(sfd,2);


	vector<int> nsfds[3];
	int popenfds[3];
	memset(popenfds,-1,sizeof(popenfds));
		
	struct pollfd fds[4];
	fds[0].fd = sfd;
	fds[0].events=fds[1].events=fds[2].events=fds[3].events = POLLIN;
	fds[0].revents=fds[1].revents=fds[2].revents=fds[3].revents = 0;
	 
	while(1){
		int ne = poll(fds,4,3*1000);
		cout << ne << endl;
		if(!ne)continue;
		
		if(fds[0].revents & POLLIN){
			int nsfd = accept(sfd,(struct sockaddr*)&address,(socklen_t*)&addrlen);
			if(nsfd < 0){
				perror("nsfd : ");
				exit(0);
			}
			int type;
			read(nsfd,&type,sizeof(type));
			cout << "yes " << type << endl;
			if(popenfds[type-1] == -1){
				char pro[] = "./D#";
				pro[3] = '0'+type;
				FILE* fp = popen(pro,"r");
				popenfds[type-1] = fileno(fp);
				fds[type].fd = fileno(fp);
			}
			nsfds[type-1].push_back(nsfd);
			continue;
		}
		for(int i = 1; i < 4; ++i){
			if(fds[i].revents & POLLIN){
				char s[1024];
				read(popenfds[i-1],s,1024);
				for(auto xx : nsfds[i-1])
					write(xx,s,strlen(s)+1);
				break;
			}
		}
	}		
	return 0;
}
	
	
	
