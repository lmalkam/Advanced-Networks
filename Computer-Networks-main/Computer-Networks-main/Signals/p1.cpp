#include <bits/stdc++.h>
using namespace std;

void hndfnPtoC(int x){
	cout << "parent sent sig\n";
	return;
}

void hndfnCtoP(int x){
	cout << "child sent sig\n";
	return;
}

int main(){
	signal(SIGUSR1, hndfnPtoC);
	signal(SIGUSR2, hndfnCtoP);

	int c = fork();
	if(c){
		while(1){
			kill(c,SIGUSR1);
			sleep(8);
		}
	}else{
		while(1){
			kill(getppid(),SIGUSR2);
			sleep(8);
		}
	}
	return 0;
}
