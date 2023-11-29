#include <bits/stdc++.h>
#include "cn_header.h"

using namespace std;

int main()
{
	sem_t *S12 = sem_open("12",O_CREAT|O_EXCL,0666,0);
	sem_t *S41 = sem_open("41",O_CREAT|O_EXCL,0666,0);
	cout<<"I am P1. Enter Any Character to sem-signal(S12): ";
	char character;
	cin>>character;
	sem_post(S12);
	cout<<"I am waiting for Semaphore S41"<<endl;
	sem_wait(S41);
	cout<<"I got semaphore signal from P4";
}
