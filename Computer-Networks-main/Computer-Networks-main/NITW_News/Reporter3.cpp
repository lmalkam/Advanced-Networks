#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
using namespace std;

int main(){
	mkfifo("./r3", 0666);
    int fd = open("./r3", O_WRONLY);
    dup2(fd,STDOUT_FILENO);
    char news[1000];

    while(cin.getline(news,sizeof(news))){
        write(STDOUT_FILENO,news,strlen(news)+1);
    }
    return 0;
}