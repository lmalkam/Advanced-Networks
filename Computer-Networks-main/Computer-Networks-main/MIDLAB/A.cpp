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

int port;
int totalCabs;
int sfd;
int DPORT;

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

char names[100][20] = {
    "Sophia", "Jackson", "Olivia", "Liam", "Emma", "Noah", "Ava", "Aiden", "Isabella", "Caden",
    "Mia", "Grayson", "Charlotte", "Elijah", "Amelia", "Mason", "Harper", "Logan", "Evelyn", "Lucas",
    "Abigail", "Ethan", "Emily", "Oliver", "Elizabeth", "Jacob", "Mila", "Ella", "Levi", "Avery",
    "Michael", "Sofia", "Alexander", "Camila", "Benjamin", "Aria", "William", "Scarlett", "Daniel", "Victoria",
    "Matthew", "Madison", "Henry", "Luna", "Joseph", "Grace", "Jackson", "Chloe", "Samuel", "Penelope",
    "Sebastian", "Riley", "David", "Layla", "Carter", "Zoey", "Wyatt", "Nora", "Jayden", "Lily",
    "John", "Ellie", "Owen", "Hannah", "Dylan", "Lila", "Luke", "Aurora", "Gabriel", "Addison",
    "Anthony", "Stella", "Isaac", "Nova", "Grayson", "Genesis", "Christopher", "Eva", "Joshua", "Emilia",
    "Andrew", "Makayla", "Lincoln", "Elena", "Mateo", "Naomi", "Ryan", "Faith", "Jaxon", "Emery",
    "Leah", "Maddison", "Zoe", "Audrey", "Nathan", "Aaliyah", "Caleb", "Bella", "Eli", "Claire"
};

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
    srand(time(0));
    totalCabs = atoi(argv[2]);
    if(argv[1][0] == 'M')port = 10000;
    else port = 11000;
    DPORT = port;
    cout << port << endl;
    sfd = sfdMaker(port);

    int nsfd;
    sockaddr_in clien;
    socklen_t cliensize = sizeof(clien);
    nsfd = accept(sfd,(sockaddr*)&clien,&cliensize);
    send(nsfd,&totalCabs,sizeof(int),0);
    char location[1000];
    while(read(nsfd,location,sizeof(location))){
        cout << location << endl;
        int portToPass = ++DPORT;
        char ptp[100];
        sprintf(ptp,"%d",portToPass);
        char driver[20];
        memset(driver,'\0',sizeof(driver));
        int idx = rand()%100;
        strcpy(driver,names[idx]);

        int c = fork();
        if(c < 0){
            perror("fork fail : ");
            exit(0);
        }
        if(!c){
            close(nsfd);
            close(sfd);
            char* pname[] = {"./T",location,driver,ptp,NULL};
            execv(pname[0],pname);
        }else{
            send(nsfd,ptp,strlen(ptp)+1,0);
        }
    }
    return 0;
}