#include<bits/stdc++.h>
#include <sys/un.h>
#include<sys/socket.h>
using namespace std;

#define socketPath "mysocket"




int main(){
    int usfd = socket(AF_UNIX,SOCK_STREAM,0);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path,socketPath);

    unlink(socketPath);
    if(bind(usfd,(sockaddr*)&addr,sizeof(addr)) < 0){
        perror("bind fail ");
        exit(0);
    }
    if(listen(usfd,5) < 0){
        perror("listen fail ");
        exit(0);
    }

    sockaddr_un clien;
    socklen_t clienlen = sizeof(clien);
    int nusfd = accept(usfd,(sockaddr*)&clien,&clienlen);

    char buff[1024];

    read(nusfd,buff,1024);
    cout << buff << endl;

    strcpy(buff,"Hello From Server");
    write(nusfd,buff,strlen(buff)+1);
    
    return 0;
}

