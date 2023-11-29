#include<bits/stdc++.h>
#include <sys/select.h>
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

	/* SELECT PART */ 
	fd_set read_fds;
			
	struct timeval timeout;
	timeout.tv_sec = 5;
	while(1){
	FD_ZERO(&read_fds);
	for(auto fd : fds)
		FD_SET(fd, &read_fds);
		int ne = select(*max_element(fds.begin(), fds.end())+1,&read_fds,NULL,NULL,NULL);
		
		if(ne){
			for(auto fd : fds){
				if(FD_ISSET(fd,&read_fds)){
					char s[buflen];
					int numbytes = read(fd,s,buflen-1);
					if(numbytes > 0){
						s[numbytes] = '\0';
						write(wfd,s,strlen(s)+1);
					}
				}
			}
		}else{
			cout << ne << endl;
			char s[] = "Hello from P1\n";
			write(wfd,s,strlen(s)+1);
		}
	}
	for(int i = 0; i < 4; ++i)
		close(fds[i]);
	return 0;
}
	
	
	
	
	
	
	
	
	
	
	
