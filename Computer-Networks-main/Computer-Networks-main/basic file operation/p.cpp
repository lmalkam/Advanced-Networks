#include<bits/stdc++.h>
#include<fcntl.h>
using namespace std;

#define buflen 1000

int main(){
	char s[buflen];
	
	/*FILE* fp = fopen("text.txt", "r");
	while(fgets(s,buflen,fp))
		cout << s << endl;	
	fclose(fp);*/
	
	int fd = open("text.txt", O_RDONLY);
	
	while(read(fd,s,buflen))
		cout << s << endl;
	
		
	return 0;
}
	
	
