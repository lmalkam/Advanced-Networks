#include <bits/stdc++.h>
#include <sys/ipc.h>
using namespace std;


int main(){
	
	char s[100];

	FILE* fp = popen("pidof ./p2", "r");
	read(fileno(fp),s,100);
	fclose(fp);
	int ppid = atoi(s);
	cout << ppid;
	kill(ppid,SIGUSR1);
	return 0;
}
