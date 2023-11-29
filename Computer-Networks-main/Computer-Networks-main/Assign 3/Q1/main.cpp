#include <bits/stdc++.h>
#include <sys/wait.h>
using namespace std;



int main(int argc, char* argv[]){
	
	int total_process = argc-1;
	ofstream of("logf.txt");
	int **pipes;
	if(total_process){
		pipes = (int**) malloc (total_process * sizeof(int*));
		for(int i = 0; i < total_process; ++i){
			pipes[i] = (int*) malloc(2* sizeof(int));
			pipe(pipes[i]);
		}
		for(int i = 0; i < total_process; ++i){
			int c = fork();
			if(!c){
				dup2(pipes[i][0],STDIN_FILENO);
				if(i != total_process-1)
					dup2(pipes[i+1][1],STDOUT_FILENO);
				char s[] = "./";
				strcat(s,argv[i+1]);
				execv(s, NULL);
			}
		}
	}
	if(total_process)dup2(pipes[0][1],1);
	string s;
	cin >> s;
	cout << "Hey " << s << endl;
	return 0;
}
