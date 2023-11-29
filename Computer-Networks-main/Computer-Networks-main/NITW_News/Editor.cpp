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

    int msqid = msgget(321, IPC_CREAT | 0666);
    
    /*int pid = getpid();
    char pidc[20];
    sprintf(pidc,"%d",pid);
    cout << pidc << endl;
    strcpy(mesg.news,pidc);
    mesg.type = 1;
    msgsnd(msqid,&mesg,sizeof(mesg.news)+1,0);
    msgsnd(msqid,&mesg,sizeof(mesg.news)+1,0);*/

    struct message_buff mesg;
    vector<int> fds;
    char fifonames[] = "./r#";
    cout << "bruh\n";
    cout.flush();
    for(int i = 1; i < 4; ++i){
        fifonames[3] = '0'+i;
        int fd = open(fifonames,O_RDONLY);
        cout << fd << endl;
        fds.push_back(fd);
    }

    struct pollfd pfds[3];
    for(int i = 0; i < 3; ++i){
        pfds[i].fd = fds[i];
        pfds[i].events = POLLIN;
        pfds[i].revents = 0;
    }

    while(1){
        int ne = poll(pfds,3,1000*5);
        if(!ne)continue;
        char news[1000];
        for(int i = 0; i < 3; ++i){
            if(pfds[i].revents & POLLIN){
                read(pfds[i].fd,news,sizeof(news));
                cout << news << endl;
                strcpy(mesg.news,news);
                mesg.type = 2;
                msgsnd(msqid,&mesg,sizeof(mesg.news)+1,0);
                cout << "Sent\n";
                break; 
            }
        }
    }
    
    return 0;
}