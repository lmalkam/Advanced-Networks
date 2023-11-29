#include<bits/stdc++.h>
#include <sys/un.h>
#include<sys/socket.h>
#include <sys/stat.h>
#include<fcntl.h>
using namespace std;

#define socketPath "mysocket"

int send_fd(int fd, int fd_to_send) {
    struct msghdr msg = { 0 };
    char buf[1];
    struct iovec iov[1] = { { buf, sizeof(buf) } };
    struct cmsghdr *cmsg;
    int *fdptr;
    char cmsgbuf[CMSG_SPACE(sizeof(int))];
    int n;

    buf[0] = 0;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_control = cmsgbuf;
    msg.msg_controllen = sizeof(cmsgbuf);

    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;

    fdptr = (int *)CMSG_DATA(cmsg);
    *fdptr = fd_to_send;

    n = sendmsg(fd, &msg, 0);

    return n;
}

int recv_fd(int fd) {
    struct msghdr msg = { 0 };
    char buf[1];
    struct iovec iov[1] = { { buf, sizeof(buf) } };
    struct cmsghdr *cmsg;
    int *fdptr;
    char cmsgbuf[CMSG_SPACE(sizeof(int))];
    int n;

    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_control = cmsgbuf;
    msg.msg_controllen = sizeof(cmsgbuf);

    n = recvmsg(fd, &msg, 0);
    if (n <= 0) {
        return n;
    }

    cmsg = CMSG_FIRSTHDR(&msg);
    fdptr = (int *)CMSG_DATA(cmsg);

    return *fdptr;
}

int main(){
    mkfifo(socketPath,0666);

    int fd = open(socketPath,O_RDONLY);
    cout << fd << endl;
    int recvfd = recv_fd(fd);
    cout << recvfd << endl;
    char lol[] = "ok lol";
    write(recvfd,lol,strlen(lol)+1);

    return 0;
}

