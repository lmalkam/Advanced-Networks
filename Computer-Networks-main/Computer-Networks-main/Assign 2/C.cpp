#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;


int main(){
	char c = 'n';
	while(c != 'y'){
		cout << "Press y to Submit, n to not, e to edit code : ";
		cin >> c;
		if(c == 'e'){
			system("gedit P.cpp");
		}
	}
	mkfifo("./famous", 0666);
	char sendCode[100000];
	int length = 0;
	
	ifstream fin("P.cpp");
	while(fin.get(c))
		sendCode[length++] = c;
	
	int fd = open("./famous", O_WRONLY);
	write(fd,sendCode,strlen(sendCode)+1);
	close(fd);
	
	
	char message[100];
	fd = open("./famous", O_RDONLY);
	read(fd,message,sizeof(message));
	close(fd);
	cout << message;
	return 0;
}
	
