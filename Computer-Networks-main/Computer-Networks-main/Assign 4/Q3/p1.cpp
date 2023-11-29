#include <bits/stdc++.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;


int main(){
	sem_t *s1, *s2;
	s1 = sem_open("/s12", O_CREAT, 0666, 0);
	s2 = sem_open("/s41", O_CREAT, 0666, 0);

	if(s1 == SEM_FAILED || s2 == SEM_FAILED){
		perror("sem err");
		return -1;
	}
	char c;
	cout << "I am P1. Enter any character to sem-signal (S12) : ";
	cin >> c;
	cout << "I am signalling semaphore signal of S12\n";
	sem_post(s1);
	
	cout << "I am waiting for semaphore S41\n";
	sem_wait(s2);
	
	cout << "I got semaphore signalling from P4\n";
	
	
	cout << "P1 continues .. :# \n";
	
	return 0;
}
	

