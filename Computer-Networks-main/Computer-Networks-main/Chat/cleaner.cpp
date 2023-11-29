#include<bits/stdc++.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pthread.h>
#include <semaphore.h>
#define f1 "./f1"
#define f2 "./f2"
#define buflen 1024
using namespace std;

sem_t *s1w, *s1r, *s2w, *s2r;

void init(){
	s1w = sem_open("/s1w", O_CREAT, 0666, 1);
	s2w = sem_open("/s2w", O_CREAT, 0666, 1);
	s1r = sem_open("/s1r", O_CREAT, 0666, 0);
	s2r = sem_open("/s2r", O_CREAT, 0666, 0);
}

int main(){
	init();
	sem_unlink("/s1w");
	sem_unlink("/s1r");
	sem_unlink("/s2w");
	sem_unlink("/s2r");
	return 0;
}
