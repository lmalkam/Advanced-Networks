#include<bits/stdc++.h>
using namespace std;


int main(){
	int c, pfd1[2], pfd2[2];
	int BUFFER_LEN = 10000;
	
	if(pipe(pfd1) == -1)return -1;
	if(pipe(pfd2) == -1)return -1;
	if((c = fork()) == -1)return -1;
	
	if(c){
		close(pfd1[0]);
		close(pfd2[1]);		
		char s[BUFFER_LEN];
		while(1){
			cout << "Parent Enter : ";
			if(cin.getline(s,BUFFER_LEN))
				write(pfd1[1],s,strlen(s)+1);
			else
				break;
			
			if(read(pfd2[0],s,BUFFER_LEN))
				cout << "From Child : " << s << endl;
			else
				break;
		}
		close(pfd1[1]);
		close(pfd2[0]);
	}
	else{
		close(pfd1[1]);
		close(pfd2[0]);
		char s1[BUFFER_LEN];
		while(1){
			if(read(pfd1[0],s1,BUFFER_LEN))
				cout << "From Parent : " << s1 << endl;
			else
				break;
			cout << "Child Enter : ";
			if(cin.getline(s1,BUFFER_LEN))
				write(pfd2[1],s1,strlen(s1)+1);
			else
				break;
		}
		close(pfd1[0]);
		close(pfd2[1]);
	}
	
	return 0;	
}	
