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

int recv_fd(int socket_fd) {
    struct iovec iov[1];
    char c = 0;
    iov[0].iov_base = &c;
    iov[0].iov_len = 1;

    struct msghdr msg;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    struct cmsghdr *cmsg;
    char buf[CMSG_SPACE(sizeof(int))];
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    int ret = recvmsg(socket_fd, &msg, 0);
    if (ret < 0)
        return ret;

    cmsg = CMSG_FIRSTHDR(&msg);
    if (cmsg == NULL || cmsg->cmsg_len != CMSG_LEN(sizeof(int)))
        return -1;
    if (cmsg->cmsg_level != SOL_SOCKET || cmsg->cmsg_type != SCM_RIGHTS)
        return -1;

    int fd = *((int *) CMSG_DATA(cmsg));
    return fd;
}

int sfdMaker(int port){
    struct sockaddr_in AT;
    int sfd;
	if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("sfd fail");
		return 0;
	}
	int opt = 1;
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        	perror("setsockopt");
        	return 0;
    }
	AT.sin_family = AF_INET;
	AT.sin_addr.s_addr = INADDR_ANY;
	AT.sin_port = htons(port);
	if(bind(sfd,(struct sockaddr*) &AT, sizeof(AT)) < 0){
		perror("bind failed");
		return 0;
	}
	if(listen(sfd,3) < 0){
		perror("listen failed");
		return 0;
	}
    return sfd;
}

int main(int argc, char* argv[]){

    char* location = argv[1];
    char* driver = argv[2];
    int port = atoi(argv[3]);
    cout << "ONPORT : " << port << endl;

    int sfd;
    sockaddr_in address;
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("sfd fail");
		exit(0);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
	
	if(connect(sfd,(struct sockaddr*)&address,sizeof(address)) < 0){
		perror("connect failed");
		exit(0);
	}

    int recvfd = sfd;
    // int recvfd = recv_fd(sfd);
    // if(recvfd < 0){
    //     perror("err : ");
    //     recvfd = sfd;
    // }
    // cout << "recvd " << recvfd << endl;

    char response[1000];// = "Your driver is : ";
    memset(response,'\0',sizeof(response));
    strcat(response,"Your Driver is : ");
    strcat(response,driver);
    strcat(response,"\nFor Location : ");
    strcat(response,location);
    send(recvfd,response,strlen(response)+1,0);

    return 0;
}