#include "cn_header.h"

using namespace std;

int main(int argc, char *argv[])
{
    mkfifo("NtoS", 0666);
    int rfd = open("./NtoS", O_RDONLY);
    while(1)
    {
        char buf[BUFFER_SIZE];
        read(rfd,buf,BUFFER_SIZE);
        cout<<"SCREEN OUTPUT:\n";
        cout<<buf;
        break;
    }
}