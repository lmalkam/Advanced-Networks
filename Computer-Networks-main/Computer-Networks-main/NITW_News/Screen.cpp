#include<bits/stdc++.h>
#include<sys/poll.h>
using namespace std;



int main(){

    int pipe1[2], pipe2[2];

    pipe(pipe1);
    pipe(pipe2);

    int c = fork();

    if(!c){
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);
        dup2(pipe1[1],STDOUT_FILENO);
        char name[] = "./NR1";
        execv(name,NULL);
    }
    c = fork();
    if(!c){
        close(pipe2[0]);
        close(pipe1[0]);
        close(pipe1[1]);
        dup2(pipe1[1],STDOUT_FILENO);
        char name[] = "./NR2";
        execv(name,NULL);
    }
    close(pipe1[1]);
    close(pipe2[1]);
    // FILE* fp1 = popen("./NR1", "r");
    // FILE* fp2 = popen("./NR2", "r");
    char news[1000];
    read(pipe1[0],news,sizeof(news));
    cout << news;
    struct pollfd pfds[2];

    pfds[0].fd = pipe1[0];
    pfds[1].fd = pipe2[0];
    pfds[0].events = pfds[1].events = POLLIN;
    pfds[0].revents = pfds[1].revents = 0;

    while(1){
        int ne = poll(pfds,2,1000*5);
        if(!ne)continue;

        if(pfds[0].revents & POLLIN){
            cout << "From NR 1" << endl;
            read(pfds[0].fd,news,sizeof(news));
            cout << news << endl;
        }
        if(pfds[1].revents & POLLIN){
            cout << "From NR 2" << endl;
            read(pfds[1].fd,news,sizeof(news));
            cout << news << endl;
        }
    }
    return 0;
}