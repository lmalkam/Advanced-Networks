#include "cn_header.h"
#define READERS 3
#define ALL_BUF 4

using namespace std;

struct msg_buf {
    long type;
    char text[BUFFER_SIZE];
} msg;

int select_fds(int fd[])
{
    int maxfd = -1;
    fd_set rst;
    for(int i=0;i<ALL_BUF;i++)
        maxfd = fd[i] < maxfd ? maxfd : fd[i];
    while(1)
    {
        FD_ZERO(&rst);
        for(int i=0; i<ALL_BUF; i++)
        {
            FD_SET(fd[i], &rst);
        }
        int res = select(maxfd, &rst, NULL, NULL, NULL);
        for(int i=0; i<ALL_BUF - 1; i++)
        {
            if( FD_ISSET(fd[i], &rst) )
            {
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    FILE* file[READERS];
    int fd[READERS];
    char readers[3][10] = {
        "./r1",
        "./r2",
        "./r3",
    };

    for(int i=0; i < READERS; i++)
    {
        file[i] = popen(readers[i], "r");
        fd[i] = fileno(file[i]);
    }
    char all_buf[5][BUFFER_SIZE];

    int fd_ip = 0;
    read(fd_ip, all_buf[0], BUFFER_SIZE);

    for(int i=0; i < READERS; i++)
    {
        int p = read(fd[i],all_buf[i+1], BUFFER_SIZE);
        all_buf[i+1][p] = '\0';
    }

    for(int i=0;i<ALL_BUF;i++)
    {
        if( all_buf[i][0] == '/' && all_buf[i][1] == 'd')
        {
            FILE *doc = fopen("D.txt","a");
            int d_fd = fileno(doc);
            int A[2];
            pipe(A);
            close(A[0]);
            dup2(d_fd,A[1]);
            write(A[1],all_buf[i],strlen(all_buf[i]));
        }
        else{
            key_t key = ftok(".", 1234);
            int msgid = msgget(key, 0666 | IPC_CREAT);
            msg.type = i%3 == 0 ? 20 : 25;
            strcpy(msg.text, all_buf[i]);
            int p = msgsnd(msgid, &msg, sizeof(msg), 0);
            cout<<p<<": "<<msg.type<<" "<<msg.text<<endl;
            cout<<"Message Added to Message Queue\n";
            sleep(1);
        }
    }
    
}