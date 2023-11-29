#include<bits/stdc++.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <semaphore.h>
#include<pthread.h>
#define buflen 1024
using namespace std;

char f1[] = "./f1";


int main(){
	mkfifo(f1,0666);
	
	int rfd = open(f1, O_RDONLY);
	
	char lol[buflen];
	read(rfd,lol,buflen);
	cout << lol;
	close(rfd);
	
	return 0;
}
	
	
	
