#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/poll.h>
#include<signal.h>

int main(){
  while(1){
    char buf[20];
    scanf("%s",buf);
    printf("%s\n",buf);
  }
}
