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

 
    if(connect(usfd,(struct sockaddr *)&addr,sizeof(addr))==-1)
	perror("\n connect ");

    char buff[1024];

    strcpy(buff,"Hello From Client");
    write(usfd,buff,strlen(buff)+1);

    read(usfd,buff,1024);
    cout << buff << endl;
    
    return 0;
}

