#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

int main(){
	sem_t *s1, *s2;
	s1 = sem_open("/s1", O_CREAT, 0666, 0);
	s2 = sem_open("/s2", O_CREAT, 0666, 0);

	key_t key1 = 12345;
	key_t key2 = 1234;
	
	int shmid1 = shmget(key1,1024,0666|IPC_CREAT);
	int shmid2 = shmget(key2,1024,0666|IPC_CREAT);
	
	
	int* x = (int*) shmat(shmid1, (void*)0, 0);
	int* y = (int*) shmat(shmid2, (void*)0, 0);
	
	
	cout << "Waiting for S1 ...\n";
	sem_wait(s1);
	
	cout << "Enter Value for x : ";
	cin >> *x;
	*y = *x+1;
	cout << "Enter any char to signal S2 : ";
	char c; cin >> c;
	sem_post(s2);
	
	
	cout << "P2 exiting ... Values of x and y are : " << *x << " " << *y << endl;

	shmdt(x);
	shmdt(y);
	
	shmctl(shmid1,IPC_RMID,NULL);
	shmctl(shmid2,IPC_RMID,NULL);
	
	return 0;
}

