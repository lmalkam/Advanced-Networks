#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
using namespace std;

int main(){
	mkfifo("./famous", 0666);
	char receiveCode[100000];
	int length = 0;
	
	int fd = open("./famous", O_RDONLY);
	read(fd,receiveCode,sizeof(receiveCode));
	close(fd);
	
	ofstream fout("temp.cpp");
	for(int i = 0; i < strlen(receiveCode); ++i)
		fout << receiveCode[i];

	fout.close();
	system("g++ temp.cpp -o P.exe");
	
	int c;
	c = fork();
	
	if(c){
		wait(NULL);
		ifstream exp("OT.txt");
		ifstream act("Pout.txt");
		string exps, acts;
		bool er = false;

		while(exp >> exps && act >> acts){
			if(exps != acts){
				er = true;
				int fd = open("./famous", O_WRONLY);
				char message[] = "Test Case Failed\n";
				write(fd,message,strlen(message)+1);
				close(fd);
				break;
			}
		}
		if(!er){
			int fd = open("./famous", O_WRONLY);
			char message[] = "Test Case Passed\n";
			write(fd,message,strlen(message)+1);
			close(fd);
		}
	}
	else{
		int itfd = open("IT.txt", O_RDONLY);
		system("touch Pout.txt");
		int poutfd = open("Pout.txt", O_WRONLY);
		dup2(itfd, 0);
		dup2(poutfd, 1);
		
		execv("./P.exe", NULL);
		
	}
	return 0;
}
	
