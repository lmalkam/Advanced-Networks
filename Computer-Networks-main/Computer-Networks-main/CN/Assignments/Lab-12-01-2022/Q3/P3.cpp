#include <bits/stdc++.h>
#include "cn_header.h"

using namespace std;
int main()
{
	sem_t *S23 = sem_open("23",O_EXCL,0666,0);
	sem_t *S34 = sem_open("34",O_CREAT|O_EXCL,0666,0);
	
	cout<<"I am P3.I am waiting for Semaphore S23.."<<endl;
	sem_wait(S23);
	cout<<"I got semaphore signal from P2"<<endl;
	cout<<"Enter Any Character to sem-signal(S34)"<<endl;
	char character;
	cin>>character;
	sem_post(S34);
}
