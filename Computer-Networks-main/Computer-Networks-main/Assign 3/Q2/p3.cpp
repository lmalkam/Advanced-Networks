#include<bits/stdc++.h>
using namespace std;


int main(){
	char s[] = "Hey this is P3\n";
	while(1){
		//sleep(rand()%9);
		write(STDOUT_FILENO,s,strlen(s)+1);
		sleep(rand()%9);
	}
	return 0;
}
