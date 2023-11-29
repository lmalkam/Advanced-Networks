#include "cn_header.h"

using namespace std;

struct msg_buf {
    long type;
    char text[BUFFER_SIZE];
    long flag;
} msg;

int main(int argc, char *argv[])
{
    key_t key = ftok(".", 1234);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    cout<<"Waiting for message\n";
    while(1)
    {
        msgrcv(msgid, &msg, sizeof(msg), 0, 0);
        int cur = 0;
        if(msg.text[0] == '0')
            cur = 4;
        cout<<"Data Received is : "<<&(msg.text[cur])<<"\n\n";

        // mkfifo("NtoS",0666);
        // int fd = open("./NtoS", O_WRONLY);

        // write(fd, msg.text, strlen(msg.text));
    }
    msgctl(msgid, IPC_RMID, NULL);
  
    return 0;
}
