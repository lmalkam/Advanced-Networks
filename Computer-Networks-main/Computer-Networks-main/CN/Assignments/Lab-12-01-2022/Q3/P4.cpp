#include <bits/stdc++.h>
#include "cn_header.h"

using namespace std;
int main()
{
	sem_t *S41 = sem_open("41",O_EXCL,0666,0);
	sem_t *S34 = sem_open("34",O_EXCL,0666,0);
	
	cout<<"I am P4.I am waiting for Semaphore S34"<<endl;
	sem_wait(S34);
	cout<<"I got semaphore signal from P3"<<endl;
	cout<<"Enter Any Character to sem-signal(S41) ";
	char ch;
	cin>>ch;
	sem_post(S41);
}
