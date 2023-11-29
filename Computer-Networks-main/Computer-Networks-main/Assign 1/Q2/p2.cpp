#include <bits/stdc++.h>
using namespace std;

void initialConnection(int &readFD, int &writeFD){
	int pipeRead = dup(0);
	int pipeWrite = dup(1);
	
	dup2(readFD, 0);
	dup2(writeFD, 1);
	
	//readFD = pipeRead;
	//writeFD = pipeWrite;
	
	dup2(pipeRead,readFD);
	dup2(pipeWrite,writeFD);

	return ;
}

int main(int argc, char* argv[]){
	int rfd = stoi(argv[1]), wfd = stoi(argv[2]);
	
	
	initialConnection(rfd,wfd);
	//cout << "I am process P2\n";
	int BUFFER_LEN = 10000;
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
			{cout << "brek\n";break;}
		
	}
	
	/*char s2[] = "Pipe go brffffffrr";
	write(wfd,s2,strlen(s2)+1);
	char s1[] = "Pipesssssss go brrr";
	write(wfd,s1,strlen(s1)+1);*/
	
	close(rfd);
	close(wfd);
	return 0;
}
