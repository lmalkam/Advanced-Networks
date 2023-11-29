#include<bits/stdc++.h>
#include<netinet/in.h>
#include<sys/poll.h>
using namespace std;


int main(int argc, char** argv){
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	int port = atoi(argv[1]);
	addr.sin_port = htons(port+1); 
	int stat = connect(sfd,(struct sockaddr*)&addr,sizeof(addr));
	if(stat < 0){
		perror("stat " );
		exit(0);
	}
	cout << "succ\n";
	char s[100];
	
	while(read(sfd,s,100))
		cout << s << endl;
	return 0;
}
				
				
	
	
	
	
