#include<bits/stdc++.h>
using namespace std;

#define buf 1024


int main(){
	int pfd1[2], pfd2[2];
	pipe(pfd1);
	pipe(pfd2);
	int c = fork();
	
	if(!c){
		close(pfd1[1]);
		close(pfd2[0]);
		int rfd = dup(STDIN_FILENO);
		int wfd = dup(STDOUT_FILENO);
		dup2(pfd1[0],STDIN_FILENO);
		dup2(pfd2[1],STDOUT_FILENO);
		string arg1 = to_string(rfd);
		string arg2 = to_string(wfd);
		char* c_arg1 = const_cast<char*>(arg1.c_str());
		char* c_arg2 = const_cast<char*>(arg2.c_str());
		char *args[] = {"p2", c_arg1, c_arg2,NULL};
		execv("./p2", args);
	}else{
		close(pfd1[0]);
		close(pfd2[1]);
		while(1){
			char s[buf];
			cout << "P1 enter : ";
			if(cin.getline(s,buf))
				write(pfd1[1],s,strlen(s)+1);
			else
				break;
			if(read(pfd2[0],s,buf))
				cout << "From P2 : " << s << endl;
			else
				break;
		}
		close(pfd1[1]);
		close(pfd2[0]);
	}
	return 0;
}
