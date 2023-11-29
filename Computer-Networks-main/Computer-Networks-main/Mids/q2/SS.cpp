#include<bits/stdc++.h>
#include <netinet/in.h>
#include<sys/poll.h>
using namespace std;

struct sockaddr_in address;


int sfdMaker(int PORT){
	int sfd = socket(AF_INET, SOCK_STREAM,0);
	address.sin_port = htons(PORT);
	int opt = 1;
	int addrlen = sizeof(address);
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));
	bind(sfd,(struct sockaddr*)&address,sizeof(address));
	listen(sfd,2);
	return sfd;
}

int main(){
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	
	vector<int> sfds;
	for(int i = 1; i < 5; ++i)
		sfds.push_back(sfdMaker(8000+i));
		
	
	struct pollfd fds[4];
	for(int i = 0; i < 3; ++i){
		 fds[i].events=POLLIN;
		 fds[i].fd = sfds[i];
		 fds[i].revents=0;
	}
		 
	while(1){
		int ne = poll(fds,3,1000*5);
		if(!ne)continue;
		
		
		for(int i = 0; i < 3; ++i){
			if(fds[i].revents & POLLIN){			
				/*struct sockaddr_in clientaddr;
				int addrlen = sizeof(clientaddr);
				int nsfd = accept(fds[i].fd,(struct sockaddr*)&clientaddr,(socklen_t*)&addrlen);
				if(nsfd < 0){
					cout << "fail\n";
				}else{
					cout << "Success at " << i << endl;
				}*/
				
				char servicename[] = {'.','/','S','#','\0'};
				servicename[3] = '1'+i;
				cout << "REDIRECTING TO " << servicename << endl;
				int c = fork();
				if(!c){
					dup2(fds[i].fd,20);
					for(auto sfd : sfds)
						if(sfd != fds[i].fd)
							close(sfd);
					
					execv(servicename,NULL);
				}
				close(fds[i].fd);
				break;
			}
		}
	}
	return 0;
}
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
