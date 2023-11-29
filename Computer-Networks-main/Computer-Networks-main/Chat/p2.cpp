#include<bits/stdc++.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pthread.h>
#include <semaphore.h>
#define buflen 1024
using namespace std;

char f1[] = "./f1";
char f2[] = "./f2";

sem_t *s1w, *s1r, *s2w, *s2r;

void init(){
	s1w = sem_open("/s1w", O_CREAT, 0666, 1);
	s2w = sem_open("/s2w", O_CREAT, 0666, 1);
	s1r = sem_open("/s1r", O_CREAT, 0666, 0);
	s2r = sem_open("/s2r", O_CREAT, 0666, 0);
}

void* reader(void* args){
	char s[buflen];
	while(1){
		int rfd = open(f2, O_RDONLY);
		int val;
		sem_getvalue(s1w,&val);
		//cout << val << endl;
		sem_wait(s1r);
		//cout << "bruhg\n";
		int bytesRead = read(rfd,s,buflen);
		if(bytesRead > 0)
			cout << "Rec : " << s << endl;
		close(rfd);
		sem_post(s1w);
	}
}

void* writer(void* args){
	char s[buflen];
	while(1){
		sem_wait(s2w);
		cin.getline(s,buflen);
		int wfd = open(f1,O_WRONLY);
		write(wfd,s,strlen(s)+1);
		close(wfd);
		sem_post(s2r);
	}
}

int main(){
	init();
	//mkfifo(f1, 0666);
	//mkfifo(f2, 0666);
	
	
	pthread_t p1,p2;
	
	pthread_create(&p1,NULL,reader,NULL);
	pthread_create(&p2,NULL,writer,NULL);
	
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	
	sem_unlink("/s1w");
	sem_unlink("/s1r");
	sem_unlink("/s2w");
	sem_unlink("/s2r");
	
	return 0;
}
	
	
	
