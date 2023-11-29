#include<bits/stdc++.h>
#include<netinet/in.h>
#include<sys/poll.h>
#include<pthread.h>
using namespace std;


void* service(void* arg){
	int nsfd = *(int*)arg;
	char s[] = "From A :)\n";
	while(1){
		send(nsfd,s,strlen(s)+1,0);
		sleep(4);
	}
}


int main(){
	int sfd = 20;
	int initNSFD = 21;
	struct sockaddr_in addr;
	
	read(22,&addr,sizeof(addr));
	
	vector<pthread_t> pts(1);
	pthread_create(&pts[0],NULL,service,(void*)&initNSFD);
	
	int nsfd;
	int addrlen = sizeof(struct sockaddr_in);
	while((nsfd = accept(sfd,(struct sockaddr*) &addr,(socklen_t*)addrlen)) > 0){
		pthread_t pt;
		pthread_create(&pt,NULL,service,(void*)&nsfd);
		pts.push_back(pt);
	}
	for(auto pt : pts)
		pthread_join(pt,NULL);

	return 0;
}
				
				
	
	
	
	
