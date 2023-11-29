#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <sys/msg.h>
#include <sys/poll.h>
using namespace std;

struct message_buff{
    char news[1000];
    long type;
};


int main(){
    cout << "NR1 Started\n";
    sem_t *s1, *s2;
	s1 = sem_open("/s1", O_CREAT, 0666, 1);
	s2 = sem_open("/s2", O_CREAT, 0666, 0);
    int msqid = msgget(321, IPC_CREAT | 0666);
    
    struct message_buff msg;
    /*msgrcv(msqid,&msg,1000,1,0);
    int pidOfEditor = atoi(msg.news);
    ofstream ofo("log.txt");
    cout << pidOfEditor;*/
    while(1){
        sem_wait(s1);
        msgrcv(msqid,&msg,1000,2,0);
        cout << msg.news << endl;
        sem_post(s2);
    }

    return 0;
}