#include <bits/stdc++.h>
using namespace std;

int c = 0;

void hndfn(int x){
	c++;
	cout << "Sike\n";
	if(c == 5)
		signal(SIGINT, SIG_DFL);
	return;
}

int main(){
	signal(SIGINT, hndfn);
	while(1){;}
	return 0;
}
