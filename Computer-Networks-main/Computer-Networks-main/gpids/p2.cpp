#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;



int main(){
	cout << getpid() << " " <<  getpgrp() << endl;

	pid_t lol;
	cin >> lol;
	
	// if(setpgid(getpid(),lol) == -1){
	// 	perror("Fail : ");
	// 	exit(0);
	// }
	// kill(lol,SIGKILL);
	pause();
	cout << "damn";
	return 0;
}
	
