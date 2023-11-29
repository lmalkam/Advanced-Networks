#include <bits/stdc++.h>
#include <sys/select.h>
#include <unistd.h>
 #include <sys/types.h>
       #include <sys/stat.h>

#include <fcntl.h>
#include <cstring>
#include <iostream>
using namespace std;

int main(){
	const char *FIFO_NAME = "./myfifo";
	mkfifo(FIFO_NAME, 0666);
	int fifo = open(FIFO_NAME, O_WRONLY);
	while(1){
		cout << "Send : ";
		string s; cin >> s;
		write(fifo,s.c_str(),s.length()+1);
	}
	return 0;
}
