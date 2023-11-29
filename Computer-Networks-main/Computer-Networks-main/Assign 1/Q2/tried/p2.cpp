#include <bits/stdc++.h>
using namespace std;

void initialConnection(int &readFD, int &writeFD){
	FILE* ptr[2];
	read(0,ptr,sizeof(ptr));
	
	FILE* rptr = fdopen(0,"r");
	FILE* wptr = fdopen(1, "w");
	dup2(0,readFD);
	dup2(1,writeFD);
	
	
	stdin = ptr[0];
	stdout = ptr[1];
	
	
	cout << "recoverded";
	//ofstream of("log.txt");
	//of << fileno(ptr[0]) << " " << fileno(ptr[1]) << endl;
	//of << ptr[0] << " " << ptr[1] << endl << stdin << " " << stdout << " " << rptr << " " << wptr << endl;
	return ;
}

int main(){
	int rfd, wfd;
	//initialConnection(rfd,wfd);

	char stdinFile[0xFFF];
	
	read(0,stdinFile,0xFFF);
	
	ofstream of("log.txt");
	of << stdinFile << endl;

	FILE *fp;
	fp = fopen(stdinFile, "r");
	int fd = fileno(fp);

	dup2(fd,0);

	cout << "recovered";
	//char init[] = "I am process P2";
	//write(wfd,init,strlen(init)+1);
	
	/*int BUFFER_LEN = 10000;
	char s[BUFFER_LEN];
	while(1){
		if(read(rfd,s,BUFFER_LEN))
			cout << "From P1 : " << s << endl;
		else
			break;
		cout << "P2 Enter : ";
		if(cin.getline(s,BUFFER_LEN))
			write(wfd,s,strlen(s)+1);
		else
			break;
	}
	cout << "p2 exit\n";*/
	close(rfd);
	close(wfd);
	
	
	return 0;
}
