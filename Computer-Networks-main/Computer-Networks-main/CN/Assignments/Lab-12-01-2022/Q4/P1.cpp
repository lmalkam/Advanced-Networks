#include <bits/stdc++.h>
#include "cn_header.h"

using namespace std;
int main()
{
	key_t key1 = ftok("ins_1.txt",'A');
	key_t key2 = ftok("ins_2.txt",'B');

	void* x =  shmat((int)shmget(key1,1024,0666|IPC_CREAT),(void*)0,0);
	void* y =  shmat((int)shmget(key2,1024,0666|IPC_CREAT),(void*)0,0);

	*(int *)x=1;
	*(int *)y=1;

	sem_t *S1 = sem_open("S1",O_EXCL,0666,0);
	sem_t *S2 = sem_open("S2",O_EXCL,0666,0);

	cout<<"I am reading shm y :"<<*(int*)y<<endl;

	// incrementing x by y+1
	*(int*)x = *(int*)y + 1;

	cout<<"Enter Any Character to signal S1\n";
	shmdt(x);
	shmdt(y);
	char character;
	cin>>character;
	sem_post(S1);

	cout<<"I am waiting for S2\n";
	sem_wait(S2);
}