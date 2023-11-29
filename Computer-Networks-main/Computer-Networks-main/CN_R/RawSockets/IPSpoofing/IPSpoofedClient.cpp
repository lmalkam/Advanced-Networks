#include"header.h"
int main()
{
    int sfd=socket(AF_INET,SOCK_RAW,10);
    perror("Socket create error\n");
    int opt=1;
    //setsockopt(sfd,IPPROTO_IP,IP_HDRINCL,&opt,sizeof(int));
    string dest_addr;
    sockaddr_in addr,caddr;
    socklen_t addrlen;
    addr.sin_family=AF_INET;
    inet_aton("127.0.0.1",&addr.sin_addr);
    bind(sfd,(sa)&addr,sizeof(addr));
    perror("Bind error\n");
    char Buffer[1024];
    memset(Buffer,'\0',sizeof(Buffer));
    while(true)
    {
            recvfrom(sfd,Buffer,1024,0,(sa)&caddr,&addrlen);
        perror("Recv error\n");
        //cout<<Buffer<<endl;
        cout<<"Address of receiver\n";
        cout<<inet_ntoa(caddr.sin_addr);
    }
}