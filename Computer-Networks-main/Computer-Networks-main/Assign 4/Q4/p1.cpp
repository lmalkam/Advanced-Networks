#include <bits/stdc++.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
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
	
	*x = 1;
	*y = 1;
	
	cout << "Enter Value for y : ";
	cin >> *y;
	*x = *y+1;
	cout << "Enter any char to signal S1 : ";
	char c; cin >> c;
	sem_post(s1);
	
	cout << "Waiting for S2 ...\n";
	sem_wait(s2);
	
	cout << "P1 exiting ... Values of x and y are : " << *x << " " << *y << endl;
	
	shmdt(x);
	shmdt(y);

	return 0;
}

