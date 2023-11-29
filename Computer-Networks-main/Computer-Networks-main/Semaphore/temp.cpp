#include <bits/stdc++.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;


int main(){
	sem_t *s1, *s2;

	s1 = sem_open("/mysem1", O_CREAT, 0666, 1);
	s2 = sem_open("/mysem2", O_CREAT, 0666, 1);

	if(s1 == SEM_FAILED || s2 == SEM_FAILED){
		perror("sem err");
		return -1;
	}
	//int t = 2;
	int c = fork();
	cout << "bruh";
	if(!c){
		for(int i = 0; i < 1; ++i){
			sem_wait(s1);
			sem_post(s1);
			
			sem_wait(s2);
			sem_post(s2);
		}
		return 0;
	
	}else{
		for(int i = 0; i < 1; ++i){
			sem_wait(s2);
			sem_post(s2);
			
			sem_wait(s1);
			sem_post(s1);
		}
		wait(NULL);
		sem_close(s1);
		sem_close(s2);
		sem_unlink("/mysem1");
		sem_unlink("/mysem2");
	}
	return 0;
}
	

