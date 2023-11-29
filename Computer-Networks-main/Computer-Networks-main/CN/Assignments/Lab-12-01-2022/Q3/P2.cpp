#include <bits/stdc++.h>
#include "cn_header.h"

using namespace std;
int main()
{
	sem_t *S23 = sem_open("23",O_CREAT|O_EXCL,0666,0);
	sem_t *S12 = sem_open("12",O_EXCL,0666,0);
	cout<<"I am P2.I am waiting for Semaphore S12\n";
	sem_wait(S12);
	cout<<"I got semaphore signal from P1"<<endl;
	cout<<"Enter Any Character to sem-signal(S23)\n";
	char character;
	cin>>character;
	sem_post(S23);
}
