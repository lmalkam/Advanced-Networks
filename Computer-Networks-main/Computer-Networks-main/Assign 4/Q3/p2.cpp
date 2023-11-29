#include <bits/stdc++.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;


int main(){
	sem_t *s1, *s2;
	s1 = sem_open("/s12", O_CREAT, 0666, 0);
	s2 = sem_open("/s23", O_CREAT, 0666, 0);

	if(s1 == SEM_FAILED || s2 == SEM_FAILED){
		perror("sem err");
		return -1;
	}
	
	cout << "I am P2. I am waiting for Semaphore S12\n";
	sem_wait(s1);
		
	cout << "I got semaphore signalling from P1\n";
	
	char c;
	cout << "Enter any character to sem-signal (S23) : ";
	cin >> c;
	cout << "I am signalling semaphore signal of S23\n";
	sem_post(s2);
	
	cout << "P2 continues .. :# \n";
	
	return 0;
}
	

