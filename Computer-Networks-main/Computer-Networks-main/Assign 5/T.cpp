#include<bits/stdc++.h>
#include <sys/un.h>
#include<sys/socket.h>
using namespace std;


int main(int argc, char* argv[]){
    char socketPath[] = "808#";
    int port;
    if(argv[1][0] == 'V')socketPath[3] = '1';
    else if(argv[1][0] == 'D')socketPath[3] = '2';
    else socketPath[3] = '3';

    int usfd = socket(AF_UNIX,SOCK_STREAM,0);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path,socketPath);

 
    if(connect(usfd,(struct sockaddr *)&addr,sizeof(addr))==-1){
        perror("\n connect ");
        exit(0);
    }

    char trainDetails[] = "ENG->A1->A2->A3->B1->B2->B3->GEN->PANTRY->LUGGAGE->END";

    int platform;

    read(usfd,&platform,sizeof(int));
    cout << "Received Platform " << platform << endl;
    write(usfd,trainDetails,strlen(trainDetails)+1);
    sleep(10);
    write(usfd,&platform,sizeof(int));
    cout << "Train Left\n";
    return 0;
}