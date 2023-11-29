#include"header.h"
int main()
{
    int sfd=socket(AF_INET,SOCK_RAW,10);
    int opt=1;
    setsockopt(sfd,IPPROTO_IP,IP_HDRINCL,&opt,sizeof(int));
    perror("SOck opt failed\n");
    string dest_addr;
    cout<<"Enter destination address\n";
    cin>>dest_addr;
    struct ip *iphdr;
    char * full;
    full=(char *) malloc(12000);
    iphdr= (struct ip *) full;
    memset(full,0,12000);
    inet_aton(dest_addr.c_str(),&iphdr->ip_dst);
    inet_aton("1.2.3.4",&iphdr->ip_src);
    char Buffer[]="Message";
    iphdr->ip_p=10;
    iphdr->ip_id=0;
    iphdr->ip_off=0;
    iphdr->ip_ttl=64;
    iphdr->ip_tos=16;
    iphdr->ip_v=0x4;
    iphdr->ip_hl=0x5;
    iphdr->ip_len=sizeof(ip);
    iphdr->ip_sum=0;
    sockaddr_in addr;
    inet_aton(dest_addr.c_str(),&addr.sin_addr);
    addr.sin_family=AF_INET;
    //full=(char *)memcpy(full,&iphdr,sizeof(ip));
    //full=(char *)memcpy(full+20,Buffer,7);
    //cout<<strlen(full)<<endl;
    for(int i=0;i<15;i++){
    int x=sendto(sfd,full,20,0,(sa) &addr,sizeof(addr));
    perror("Send error");
    cout<<x<<" Bytes sent \n";
    sleep(4);
    }
}