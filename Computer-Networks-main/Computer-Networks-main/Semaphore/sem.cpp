#include <bits/stdc++.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;


int main(){
	sem_t *s1, *s2;
	
	//sem_unlink("/mysem1");
	//sem_unlink("/mysem2");
	s1 = sem_open("/mysem1", O_CREAT, 0666, 1);
	s2 = sem_open("/mysem2", O_CREAT, 0666, 1);

	if(s1 == SEM_FAILED || s2 == SEM_FAILED){
		perror("sem err");
		return -1;
	}
	//int t = 2;
	int c = fork();
	if(!c){
		for(int i = 0; i < 2; ++i){
			sem_wait(s1);
			ifstream fx("x.txt");
			int x; fx >> x;
			fx.close();
			sem_post(s1);
			
			sem_wait(s2);
			ifstream fy("y.txt");
			int y; fy >> y;
			fy.close();
			ofstream fyy("y.txt");
			fyy << x+y;
			fyy.close();
			sem_post(s2);
		}
		return 0;
	
	}else{
		for(int i = 0; i < 2; ++i){
			sem_wait(s2);
			ifstream fy("y.txt");
			int y; fy >> y;
			fy.close();
			sem_post(s2);
			
			sem_wait(s1);
			ifstream fx("x.txt");
			int x; fx >> x;
			fx.close();
			ofstream fxx("x.txt");
			fxx << x+y;
			fxx.close();
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
	

