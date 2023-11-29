#include<bits/stdc++.h>

using namespace std;

#define buflen 1000


int main(){
	int pfd1[2],pfd2[2];
	pipe(pfd1);
	pipe(pfd2);
	int c = fork();
	if(c){
		close(pfd1[0]);
		close(pfd2[1]);
		while(1){
			char s[buflen];
			if(cin.getline(s,buflen))
				write(pfd1[1], s, strlen(s)+1);
			else
				break;
			char s1[buflen];
			if(read(pfd2[0],s1,buflen))
				cout << "From Child : " << s1 << endl;
			else
				break;
		}
		close(pfd2[0]);
		close(pfd1[1]);
	}else{
		close(pfd2[0]);
		close(pfd1[1]);
		while(1){
			char s[buflen];			
			if(read(pfd1[0],s,buflen))
				cout << "From Parent : " << s << endl;
			else
				break;
			char s1[buflen];	
			if(cin.getline(s1,buflen))
				write(pfd2[1], s1, strlen(s1)+1);
			else
				break;
		}	
		close(pfd1[0]);
		close(pfd2[1]);
	}
		
	return 0;
}		
		
