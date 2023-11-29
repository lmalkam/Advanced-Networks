#include<bits/stdc++.h>
#include<sys/poll.h>
#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<iostream>
#include<semaphore.h>
#include<cctype>
#include<sys/stat.h>
using namespace std;

int lol = -1;

void hndfn(int sig, siginfo_t *siginfo, void* context){
    lol = 1;
    sleep(10);
}



int main(int argc, char* argv[]){
    struct sigaction act;
    act.sa_sigaction = hndfn;
    act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);

    cout << getpid() << endl;
    cout << lol << endl;
    pause();
    cout << lol << endl;

    return 0;
}