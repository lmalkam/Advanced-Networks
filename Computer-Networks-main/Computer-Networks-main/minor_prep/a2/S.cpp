#include<bits/stdc++.h>
#include<sys/types.h>	//dk
#include<sys/wait.h>	//wait
#include<fcntl.h>	//open nd all
#include <sys/stat.h> // mkfifo
using namespace std;

#define buf 100000

int main(){
	int res = mkfifo("./famous", 0666);
	char s[buf];
	int fd = open("./famous", O_RDONLY);
	read(fd,s,sizeof(s));
	close(fd);
	int len = strlen(s);
	ofstream fout("temp.cpp");
	for(int i = 0; i < len; ++i)
		{fout << s[i];cout<<s[i];}
	fout.close();
	//system("g++ temp.cpp -o t");
	/*mkfifo("./famous", 0666);
	char receiveCode[100000];
	int length = 0;
	
	int fd = open("./famous", O_RDONLY);
	read(fd,receiveCode,sizeof(receiveCode));
	close(fd);
	
	ofstream fout("temp.cpp");
	for(int i = 0; i < strlen(receiveCode); ++i)
		fout << receiveCode[i];

	fout.close();*/
	system("g++ temp.cpp -o P.exe");
		
	int c = fork();
	if(c){
		wait(NULL);
		cout << "Passed12\n";
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
		
	}else{
		cout << "Passed1\n";
		int rfd =  open("IT.txt", O_RDONLY);
		system("touch Pout.txt");
		int wfd =  open("Pout.txt", O_WRONLY);
		dup2(rfd,STDIN_FILENO);
		dup2(wfd,STDOUT_FILENO);
		
		//char* args[] = {"./t", NULL};
		execv("./P.exe", NULL);
		
	}
	
	return 0;
}
