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
#define socketPath "mySocket"

int main(int argc, char* argv[]){


    int shmid1 = shmget(1234,sizeof(int),0666|IPC_CREAT);
    int shmid2 = shmget(12345,sizeof(int),0666|IPC_CREAT);
    int* headpid = (int*)shmat(shmid1,NULL,0);
    int* trailpid = (int*)shmat(shmid2,NULL,0);
    *headpid = 0;
    *trailpid = 0;
    return 0;
}