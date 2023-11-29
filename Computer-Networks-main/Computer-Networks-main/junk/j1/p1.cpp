#include<bits/stdc++.h>
#include<sys/stat.h>
#include<fcntl.h>
#define buflen 1024
using namespace std;

char f1[] = "./f1";


int main(){
	mkfifo(f1,0666);
	int wfd = open(f1, O_WRONLY);
	char lol[buflen];
	cin.getline(lol,buflen);
	write(wfd,lol,strlen(lol)+1);
	close(wfd);
	return 0;
}
	
	
	
