#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/poll.h>
using namespace std;

struct message_buff{
    char news[1000];
    long type;
};


int main(){

    int msqid = msgget(12345, IPC_CREAT | 0666);

    struct message_buff mesg;
    while(1){
        msgrcv(msqid,&mesg,sizeof(mesg.news)+1,1,0); 
        cout << mesg.news << endl;
    }
    
    return 0;
}