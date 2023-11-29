#include<bits/stdc++.h>
#include <sys/poll.h>
using namespace std;

#define buflen 100

int main(){
	vector<FILE*> fps;
	vector<int> fds;
	char processName[] = "./p#";
	
	for(int i = 2; i < 6; ++i){
		processName[3] = '0'+i;
		FILE* fp = popen(processName, "r");
		fps.push_back(fp);
		fds.push_back(fileno(fp));
	}

	processName[3] = '6';
	FILE* fp6 = popen(processName, "w");
	int wfd = fileno(fp6);

	/* POLL PART */ 
	struct pollfd pfds[4];
	memset(pfds, 0, sizeof(pfds));

	for(int i = 0; i < 4; ++i){
		pfds[i].fd = fds[i];
		pfds[i].events |= POLLIN;
		pfds[i].revents = 0;
	}

	while(1){
		int ne = poll(pfds, 4, 5000);
		cout << ne << endl;
		if(ne){
			for(int i = 0; i < 4; ++i){
				if(pfds[i].revents & POLLIN){
					char s[buflen];
					int numbytes = read(fds[i],s,buflen-1);
					if(numbytes > 0){
						s[numbytes] = '\0';
						write(wfd,s,strlen(s)+1);
					}
					break;
				}
			}
		}
		else{
			char s[] = "Hello from P1\n";
			write(wfd,s,strlen(s)+1);
		}
	}
	
	for(int i = 0; i < 4; ++i)
		close(fds[i]);
	return 0;
}
	
	
	
	
	
	
	
	
	
	
	
