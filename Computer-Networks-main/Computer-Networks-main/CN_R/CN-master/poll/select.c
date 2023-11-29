#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/poll.h>
#include<signal.h>
#include<sys/select.h>
#include<errno.h>

int main(){
  FILE* file2 = popen("./poll-p2","r");
	perror("Error here is 1:");
  int fd2 = fileno(file2);
  perror("Error here is 2:");
  FILE* file3 = popen("./poll-p3","r");
perror("Error here is 3:");
  int fd3 = fileno(file3);
perror("Error here is 4:");
  FILE* file6 = popen("./poll-p6","w");
perror("Error here is 5:");
  int fd6 = fileno(file6);
perror("Error here is 6:");

  while(1){
    char buf[20];
    fd_set rfds;
    fd_set wfds;

    FD_ZERO(&rfds);
perror("Error here is 7:");
    FD_SET(fd2,&rfds);
perror("Error here is 8:");
    FD_SET(fd3,&rfds);
perror("Error here is 9:");

    struct timeval tv;
    tv.tv_sec=5;
    tv.tv_usec=0;

    FD_ZERO(&wfds);
    FD_SET(fd6,&wfds);

    int s = select(6,&rfds,&wfds,NULL,&tv);
perror("Error here is 10:");
    printf("value os s-%d\n",s);

    if(s>0){
      if(FD_ISSET(fd2,&rfds)){
          dup2(fd2,0);
          scanf("%s",buf);
          fflush(0);
          printf("%s\n",buf);
      }
      if(FD_ISSET(fd3,&rfds)){
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
