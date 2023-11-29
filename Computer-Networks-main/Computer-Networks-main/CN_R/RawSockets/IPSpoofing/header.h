#include<bits/stdc++.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/un.h>
#include<arpa/inet.h>
#include<netinet/ip.h>
#include<pthread.h>
#include<poll.h>
#include<sys/msg.h>
using namespace std;
typedef sockaddr * sa;
unsigned short csum(unsigned short *ptr,int nbytes) 
{
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum=0;
	while(nbytes>1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;
	
	return(answer);
}
void setAddress(struct sockaddr_in &Addr, int portno)
{
    Addr.sin_addr.s_addr=INADDR_ANY;
    Addr.sin_port=htons(portno);
    Addr.sin_family=AF_INET;
}
int createTCPSocket(int port, int nolisten)
{
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in saddr;
    setAddress(saddr,port);
    int opt=1;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(int));
    int p=bind(sfd,(sa) &saddr,sizeof(saddr));
    if(p<0)
    perror("Error in binding to port \n");
    listen(sfd,nolisten);
    return sfd;
}
int createUDPSocket(int port)
{
    int sfd=socket(AF_INET,SOCK_DGRAM,0);
    sockaddr_in saddr;
    setAddress(saddr,port);
    int opt=1;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(int));
    int p=bind(sfd,(sa) &saddr,sizeof(saddr));
    if(p<0)
    perror("Error in binding to port \n");
    return sfd;
}
int createUXSocket(string path,int nolisten)
{
    int usfd=socket(AF_LOCAL,SOCK_STREAM,0);
    sockaddr_un uaddr;
    uaddr.sun_family=AF_LOCAL;
    strcpy(uaddr.sun_path,path.c_str());
    unlink(path.c_str());
    int p=bind(usfd,(sa) &uaddr,sizeof(uaddr));
    if(p<0)
    perror("Unix socket bind error\n");
    listen(usfd,nolisten);
    return usfd;
}
int createUXSocket(string path)
{
    int usfd=socket(AF_LOCAL,SOCK_STREAM,0);
    sockaddr_un uaddr;
    uaddr.sun_family=AF_LOCAL;
    strcpy(uaddr.sun_path,path.c_str());
    unlink(path.c_str());
    int p=bind(usfd,(sa) &uaddr,sizeof(uaddr));
    if(p<0)
    perror("Unix socket bind error\n");
    return usfd;
}
void setAddress(sockaddr_un &usaddr, string path)
{
    usaddr.sun_family=AF_LOCAL;
    strcpy(usaddr.sun_path,path.c_str());
}
ssize_t sock_fd_write(int sock, void *buf, ssize_t buflen, int fd)
{
    ssize_t     size;
    struct msghdr   msg;
    struct iovec    iov;
    union {
        struct cmsghdr  cmsghdr;
        char        control[CMSG_SPACE(sizeof (int))];
    } cmsgu;
    struct cmsghdr  *cmsg;

    iov.iov_base = buf;
    iov.iov_len = buflen;

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    if (fd != -1) {
        msg.msg_control = cmsgu.control;
        msg.msg_controllen = sizeof(cmsgu.control);

        cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_len = CMSG_LEN(sizeof (int));
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;

        printf ("passing fd %d\n", fd);
        *((int *) CMSG_DATA(cmsg)) = fd;
    } else {
        msg.msg_control = NULL;
        msg.msg_controllen = 0;
        printf ("not passing fd\n");
    }

    size = sendmsg(sock, &msg, 0);

    if (size < 0)
        perror ("sendmsg");
    return size;
}
ssize_t sock_fd_read(int sock, void *buf, ssize_t bufsize, int *fd)
{
    ssize_t     size;

    if (fd) {
        struct msghdr   msg;
        struct iovec    iov;
        union {
            struct cmsghdr  cmsghdr;
            char        control[CMSG_SPACE(sizeof (int))];
        } cmsgu;
        struct cmsghdr  *cmsg;

        iov.iov_base = buf;
        iov.iov_len = bufsize;

        msg.msg_name = NULL;
        msg.msg_namelen = 0;
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_control = cmsgu.control;
        msg.msg_controllen = sizeof(cmsgu.control);
        size = recvmsg(sock,&msg,0);
        if (size < 0) {
            perror ("recvmsg");
            exit(1);
        }
        cmsg = CMSG_FIRSTHDR(&msg);
        if (cmsg && cmsg->cmsg_len == CMSG_LEN(sizeof(int))) {
            if (cmsg->cmsg_level != SOL_SOCKET) {
                fprintf (stderr, "invalid cmsg_level %d\n",
                     cmsg->cmsg_level);
                exit(1);
            }
            if (cmsg->cmsg_type != SCM_RIGHTS) {
                fprintf (stderr, "invalid cmsg_type %d\n",
                     cmsg->cmsg_type);
                exit(1);
            }

            *fd = *((int *) CMSG_DATA(cmsg));
            printf ("received fd %d\n", *fd);
        } else
            *fd = -1;
    } else {
        size = read (sock, buf, bufsize);
        if (size < 0) {
            perror("read");
            exit(1);
        }
    }
    return size;
}
