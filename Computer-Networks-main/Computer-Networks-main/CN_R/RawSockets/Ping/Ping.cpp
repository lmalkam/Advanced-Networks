#include"header.h"
struct Pinger
{
    icmp IC;
    char Message[20];
};
int main(int argc, char * argv[])
{
    int sfd=socket(AF_INET ,SOCK_RAW,IPPROTO_ICMP);
    if(sfd<0)
    perror("Socket create error\n");
    int ttlval=64;
    char * destaddr=argv[1];
    sockaddr_in daddr;
    daddr.sin_family=AF_INET;
    inet_aton(destaddr,&daddr.sin_addr);
    Pinger Pckt;
    Pckt.IC.icmp_type=ICMP_ECHO;
    Pckt.IC.icmp_code=0;
    Pckt.IC.icmp_cksum=csum((unsigned short *)&Pckt,sizeof(Pckt));
    while(true)
    {
        Pinger Rpckt;
        sleep(3);
        sockaddr_in raddr;
        socklen_t daddrlen;
        int p=setsockopt(sfd,SOL_IP,IP_TTL,&ttlval,sizeof(int));
        if(p<0)
        perror("Sock opt error \n");
        struct timespec tval,tval2;
        clock_gettime(CLOCK_MONOTONIC,&tval);
        double starttime=(double)tval.tv_nsec/1000000 + tval.tv_sec*1000;
        sendto(sfd,&Pckt,sizeof(Pckt),0,(sa) &daddr,sizeof(daddr));
        perror("Send error\n");
        recvfrom(sfd,&Rpckt,sizeof(Pckt),0,(sa)&raddr,&daddrlen);
        perror("Recv error\n");
        cout<<"Packet received "<<Rpckt.Message<<endl;
        if(Rpckt.IC.icmp_type==ICMP_ECHOREPLY) 
        cout<<"Might be correct \n";
        clock_gettime(CLOCK_MONOTONIC,&tval2);
        double endtime= (double)tval2.tv_nsec/1000000 + tval2.tv_sec*1000;
        cout<<"Ping received from destination with time "<<endtime-starttime<<endl;
    }
} 