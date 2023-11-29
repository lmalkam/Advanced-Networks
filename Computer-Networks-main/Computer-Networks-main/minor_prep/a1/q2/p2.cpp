#include<bits/stdc++.h>
using namespace std;

#define buf 1024


int main(int argc, char* args[]){
	int stdrfd = atoi(args[1]);
	int stdwfd = atoi(args[2]);
	
	int rfd = dup(STDIN_FILENO);
	int wfd = dup(STDOUT_FILENO);
	
	dup2(stdrfd,STDIN_FILENO);
	dup2(stdwfd,STDOUT_FILENO);
	
	while(1){
		char s[buf];
		if(read(rfd,s,buf))
			cout << "From P1 : " << s << endl;
		else
			break;
		cout << "P2 enter : ";
		if(cin.getline(s,buf))
			write(wfd,s,strlen(s)+1);
		else
			break;
	}
	close(wfd);
	close(rfd);
	return 0;
}
