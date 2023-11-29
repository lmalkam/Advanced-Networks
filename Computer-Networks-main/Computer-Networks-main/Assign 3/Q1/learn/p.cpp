#include <bits/stdc++.h>
using namespace std;



int main(){
	FILE* fp;
	
	fp = popen("./p2", "w");

	if (fp == NULL) {
    		printf("Failed to run command\n" );
    		exit(1);
  	}
  	
  	for(int i = 1; i < 101; ++i)
  		cout << i << " ";
  	cout << "loln\n";
  		
  	for(int i = 101; i < 201; ++i)
  		fprintf(fp, (to_string(i) + " ").c_str());
  	return 0;
}
