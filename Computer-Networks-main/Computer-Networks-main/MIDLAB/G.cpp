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

int portM = 8001;
int portS = 8002;

int send_fd(int socket_fd, int fd_to_send) {
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

    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    *((int *) CMSG_DATA(cmsg)) = fd_to_send;

    return sendmsg(socket_fd, &msg, 0);
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

int AsfdMaker(int port){
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
    cout << "Agent " << port << " COnnected\n";
    return sfd;
}

int main(){

    int shmid1 = shmget(111,sizeof(int),0666|IPC_CREAT);
    int* M = (int*)shmat(shmid1,NULL,0);
    int shmid2 = shmget(112,sizeof(int),0666|IPC_CREAT);
    int* S = (int*)shmat(shmid2,NULL,0);


    int MAsfd = AsfdMaker(10000);
    int SAsfd = AsfdMaker(11000);

    int totalM, totalS;
    read(MAsfd,&totalM,sizeof(int));
    read(SAsfd,&totalS,sizeof(int));

    // if(*M == -1){
        *M = totalM;
        *S = totalS;
    // }

    int Msfd = sfdMaker(portM);
    if(Msfd > 0)cout << "MSFD listenting " << portM<< " " << Msfd << endl;
    int Ssfd = sfdMaker(portS);
    if(Ssfd > 0)cout << "Ssfd listenting " << portS<< " " << Ssfd<< endl;

    struct pollfd pfds[2];
    pfds[0].fd = Msfd;
    pfds[1].fd = Ssfd;

    pfds[0].events = pfds[1].events = POLLIN;
    pfds[0].revents = pfds[1].revents = 0;

    
    while(1){
        int ne = poll(pfds,2,1000*5);
        if(!ne)continue;
        cout << *M << " cab " << *S << endl;
        if(pfds[0].revents & POLLIN){
            printf("HERE 0\n");
            int nsfd;
            sockaddr_in clien;
            socklen_t cliensize = sizeof(clien);
            nsfd = accept(pfds[0].fd,(sockaddr*)&clien,&cliensize);
            printf("nsfd : %d",nsfd);
            if(*M){
                *M = *M-1;
                int resp = 1;
                send(nsfd,&resp,sizeof(int),0);
                char location[100];
                read(nsfd,location,sizeof(location));
                printf("Req for %d %s", 0, location);
                send(MAsfd,location,strlen(location)+1,0);
                char portToConnect[100];
                read(MAsfd,portToConnect,sizeof(portToConnect));
                int ptc = atoi(portToConnect);
                int sfdForTaxi = sfdMaker(ptc);

                int nsnsfd;
                sockaddr_in clien;
                socklen_t cliensize = sizeof(clien);
                nsnsfd = accept(sfdForTaxi,(sockaddr*)&clien,&cliensize);

                

                char response[1000];
                read(nsnsfd,response,sizeof(response));
                send(nsfd,response,strlen(response)+1,0);
            }else{
                int resp = 0;
                send(nsfd,&resp,sizeof(int),0);
            }
        }
        if(pfds[1].revents & POLLIN){
            printf("HERE 1\n");
            int nsfd;
            sockaddr_in clien;
            socklen_t cliensize = sizeof(clien);
            nsfd = accept(pfds[1].fd,(sockaddr*)&clien,&cliensize);
            // cout << "nsfd : " << nsfd << endl;
            printf("nsfd : %d",nsfd);
            if(*S){
                *S = *S-1;
                int resp = 1;
                send(nsfd,&resp,sizeof(int),0);
                char location[100];
                read(nsfd,location,sizeof(location));
                printf("Req for %d %s", 1, location);
                send(SAsfd,location,strlen(location)+1,0);
                char portToConnect[100];
                read(SAsfd,portToConnect,sizeof(portToConnect));
                int ptc = atoi(portToConnect);
                int sfdForTaxi = sfdMaker(ptc);

                int nsnsfd;
                sockaddr_in clien;
                socklen_t cliensize = sizeof(clien);
                nsnsfd = accept(sfdForTaxi,(sockaddr*)&clien,&cliensize);

                

                char response[1000];
                read(nsnsfd,response,sizeof(response));
                send(nsfd,response,strlen(response)+1,0);
            }else{
                int resp = 0;
                send(nsfd,&resp,sizeof(int),0);
            }
        }

    }



    return 0;
}