#include <bits/stdc++.h>
#include<unistd.h>
#include <fcntl.h>
#include<stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include <semaphore.h>
using namespace std;
int main()
{
	key_t key1 = ftok("ins_1.txt",'A');
	key_t key2 = ftok("ins_2.txt",'B');

	void* x =  shmat((int)shmget(key1,1024,0666|IPC_CREAT),(void*)0,0);
	void* y =  shmat((int)shmget(key2,1024,0666|IPC_CREAT),(void*)0,0);

	sem_t *S1 = sem_open("S1",O_EXCL,0666,0);
	sem_t *S2 = sem_open("S2",O_EXCL,0666,0);
	cout<<"I am waiting for S1\n";
	sem_wait(S1);
	cout<<"I am reading shm x:"<<*(int*)x<<endl;
	cout<<"New value of y:"<<*(int*)y<<endl;
	*(int*)y = *(int*)x + 1;
	cout<<"value of x:"<<*(int*)x<<endl;
	cout<<"value of y:"<<*(int*)y<<endl;
	shmdt(x);
	shmdt(y);
	cout<<"Enter Any Character to signal S2\n";
	char character;
	cin>>character;
	sem_post(S2);
	
}