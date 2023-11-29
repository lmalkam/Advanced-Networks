#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
using namespace std;

int main(){
	int stat = mkfifo("./r1", 0666);
    if(stat < 0){
        perror("bruh : ");
    }
    int fd = open("./r1", O_WRONLY);
    cout << fd << endl;
    dup2(fd,STDOUT_FILENO);
    char news[1000];

    while(cin.getline(news,sizeof(news))){
        write(STDOUT_FILENO,news,strlen(news)+1);
    }
    return 0;
}