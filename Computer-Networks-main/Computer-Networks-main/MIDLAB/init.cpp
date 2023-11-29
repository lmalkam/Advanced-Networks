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

int main(){

    int shmid1 = shmget(111,sizeof(int),0666|IPC_CREAT);
    int* M = (int*)shmat(shmid1,NULL,0);
    int shmid2 = shmget(112,sizeof(int),0666|IPC_CREAT);
    int* S = (int*)shmat(shmid2,NULL,0);

    *M = -1;
    *S = -1;

    return 0;
}