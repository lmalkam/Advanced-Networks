#include <bits/stdc++.h>
using namespace std;

char* convertToCharArray(int num){
	int len = floor(log10(num)) + 1;
	char* res = (char *) malloc(len * sizeof(char));
	int i = 0;
	while(num){
		res[i] = (num%10)+'0';
		i++;
		num /= 10;
	}
	for(int i = 0; i< len/2; ++i)
		swap(res[i],res[len-1-i]);
	return res;
}

int main(){
	int pid,pfd1[2],pfd2[2];
	if(pipe(pfd1) == -1)return -1;
	if(pipe(pfd2) == -1)return -1;
	if((pid = fork()) == -1)return -1;
	
	if(pid){
		close(pfd1[0]);
		close(pfd2[1]);

		int BUFFER_LEN = 10000;
		char s[BUFFER_LEN];
		while(1){
			cout << "P1 Enter : ";
			if(cin.getline(s,BUFFER_LEN))
				write(pfd1[1],s,strlen(s)+1);
			else 
				break;
			
			if(read(pfd2[0],s,BUFFER_LEN))
				cout << "From P3 via P2 : " << s << endl;
			else
				break;
		}
		close(pfd1[1]);
		close(pfd2[0]);
	}
	else{
		close(pfd1[1]);
		close(pfd2[0]);
		int savestdin = dup(0);
		int savestdout = dup(1);
		dup2(pfd1[0],0);
		dup2(pfd2[1],1);

		char* argv[] = {"p2", convertToCharArray(savestdin), convertToCharArray(savestdout), NULL};
        	execv("./p2", argv);
	}
	return 0;
}	
		
		
