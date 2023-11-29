#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/poll.h>
#include<signal.h>

int main(){
  FILE* file2 = popen("./poll-p2","r");
  int fd2 = fileno(file2);
  // dup2(fd2,0);
  FILE* file3 = popen("./poll-p3","r");
  int fd3 = fileno(file3);

  FILE* file6 = popen("./poll-p6","w");
  int fd6 = fileno(file6);

  while(1){
    char buf[20];
    struct pollfd pfd[2];
    pfd[0].fd = fd2;
    pfd[0].events = POLLIN;

    pfd[1].fd = fd3;
    pfd[1].events = POLLIN;

    int s = poll(pfd,2,10);
    if(s>0){
      if(pfd[0].revents & POLLIN){
          dup2(fd2,0);
          scanf("%s",buf);
          fflush(0);
          printf("%s\n",buf);
      }
      if(pfd[1].revents & POLLIN){
          dup2(fd3,0);
          scanf("%s",buf);
          fflush(0);
          printf("%s\n",buf);
      }
    }
    else{
        printf("from process p1\n");
    }
    // sleep(2);
  }
  // char buf[3];
  // scanf("%s",buf);
  // read(fd3,buf,3);
  // fgets(buf,2,file2);
  // printf("%s\n",buf);
}
