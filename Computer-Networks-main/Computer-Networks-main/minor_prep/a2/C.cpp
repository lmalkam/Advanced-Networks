#include<bits/stdc++.h>
#include<fcntl.h>	//open nd all
#include <sys/stat.h> // mkfifo
using namespace std;

#define buf 100000
int main(){
	char c = 'n';
		int res = mkfifo("./famous", 0666);
	if(res < 0){
		perror("Error creating fifo");
		return -1;
	}
	while(c != 'y'){
		cin >> c;
		if(c == 'e')
			system("gedit P.cpp");
	}
	
	ifstream fin("P.cpp");	
	char s[buf];
	int len = 0;
	while(fin.get(c))
		s[len++] = c;
	
	int fd = open("./famous", O_WRONLY);
	write(fd,s,strlen(s)+1);
	close(fd);
	
	sleep(5);
	fd = open("./famous", O_RDONLY);
	read(fd,s,buf);
	cout << s;
	return 0;
}	
	
	 
	
	
	
