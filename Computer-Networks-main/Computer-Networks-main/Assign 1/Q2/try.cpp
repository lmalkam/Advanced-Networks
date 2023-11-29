#include <bits/stdc++.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
using namespace std;


int main(){
	int c;
	c = fork();
	if(!c){
		cout << stdin << endl;
		int fd = open("ok.txt", O_WRONLY);
		FILE* ptr = fdopen(fd,"w");
		cout << ptr << endl;
		dup2(fd,1);
		execv("./fin",NULL);
	}
	return 0;
}
