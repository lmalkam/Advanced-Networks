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
#include<ctype.h>

int main(int argc, char const *argv[])
{
	key_t key;
	key=ftok("shmfile",65);
	int msgid=shmget(key,sizeof(int),0666|IPC_CREAT);
	int *p=(int*)shmat(msgid,0,0);

	*p = 0;
	return 0;
}