#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;



int main(){
	cout << getpid() << " " <<  getpgrp() << endl;
	
	char c = '#';
	
	while(c != 'n'){
		cout << getpgrp() << endl;
		cin >> c;
	}
	int lol; cin >> lol;
	kill(lol,SIGKILL);
	// killpg(getpgrp(),SIGUSR1);
	return 0;
}
	
