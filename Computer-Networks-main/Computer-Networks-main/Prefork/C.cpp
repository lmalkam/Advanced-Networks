#include<bits/stdc++.h>
#include<netinet/in.h>
#define PORT 8080
using namespace std;

int main(){
	struct sockaddr_in addr;
	
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	
	int stat = connect(sfd,(struct sockaddr*)&addr,sizeof(addr));
	if(stat<0){
		perror("stat : ");
		exit(1);
	}
	cout << "Succ\n";
	char s[100];
	while(read(sfd,s,sizeof(s))){
		cout << s << endl;
	}
	return 0;
}
	
	
	
