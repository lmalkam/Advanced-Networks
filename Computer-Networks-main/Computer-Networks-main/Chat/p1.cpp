#include<bits/stdc++.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <semaphore.h>
#include<pthread.h>
#define buflen 1024
using namespace std;

char f1[] = "f1";
char f2[] = "f2";

sem_t *s1w, *s1r, *s2w, *s2r;

void init(){
	s1w = sem_open("/s1w", O_CREAT, 0666, 1);
	s2w = sem_open("/s2w", O_CREAT, 0666, 1);
	s1r = sem_open("/s1r", O_CREAT, 0666, 0);
	s2r = sem_open("/s2r", O_CREAT, 0666, 0);
	if(s1w == SEM_FAILED || s1r == SEM_FAILED || s2w == SEM_FAILED || s2w == SEM_FAILED)
		perror("sem fail");
}

void* reader(void* arg){
	char s[buflen];
	while(1){
		int rfd = open(f1, O_RDONLY);
		sem_wait(s2r);
		int bytesRead = read(rfd,s,buflen);
		if(bytesRead > 0)
			cout << "Rec : " << s << endl;
		close(rfd);
		sem_post(s2w);
	}
}

void* writer(void* arg){
	char s[buflen];
	while(1){
		//cout << "bruh\n";
		int val;
		//sem_getvalue(s1w,&val);
		//cout << val << endl;
		sem_wait(s1w);
		//cout << "bruhg\n";
		cin.getline(s,buflen);
		//cout << s << endl;
		//cout << f2 << endl;
		int wfd = open(f2,O_WRONLY);
		//cout << "open succ\n";
		write(wfd,s,strlen(s)+1);
		close(wfd);
		sem_post(s1r);
	}
}

int main(){
	//cout << "yey\n";
	init();
	//cout << "ye\n";
	int res1 = mkfifo(f1, 0666);
	if(res1 < 0){
		perror("fail1 fifo");
		return 0;
	}
	int res2 = mkfifo(f2, 0666);
	if(res2 < 0){
		perror("fail2 fifo");
		return 0;
	}
	//cout << "ne\n";
	pthread_t p1,p2;
	
	pthread_create(&p1,NULL,reader,NULL);
	pthread_create(&p2,NULL,writer,NULL);
	
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	
	cout << "Damn\n";
	
	sem_unlink("/s1w");
	sem_unlink("/s1r");
	sem_unlink("/s2w");
	sem_unlink("/s2r");
	return 0;
}
	
	
	
