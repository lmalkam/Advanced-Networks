#include<bits/stdc++.h>
#include <netinet/in.h>

using namespace std;

#define PORT 8000
int main(){
	
	int nsfd = 20;
	
	char s[100];
	read(nsfd,s,100);
	cout << "Read in D : ";
	
	cout << s;
	
	cin.getline(s,100);
	write(20,s,100);
	
	
	return 0;
}
	
	
	
